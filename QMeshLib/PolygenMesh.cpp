// PMBody.cpp: implementation of the PMBody class.
//
//////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_DEPRECATE

#include <math.h>
#include <memory.h>

#include "PolygenMesh.h"

#include <QDebug>

#define PI		3.141592654
#define DEGREE_TO_ROTATE(x)		0.0174532922222*x
#define ROTATE_TO_DEGREE(x)		57.295780490443*x

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PolygenMesh::PolygenMesh(mesh_type type)
{
    ClearAll();
    m_drawListID=-1;
    m_bVertexNormalShading=false;
    isTransparent = false;
    m_drawListNumber = 6;
    meshType = type;
}

PolygenMesh::~PolygenMesh()
{
    ClearAll();
    if (m_drawListID!=-1) glDeleteLists(m_drawListID, m_drawListNumber);
}

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

void PolygenMesh::CompBoundingBox(double boundingBox[])
{
    GLKPOSITION PosMesh;
    GLKPOSITION Pos;
    double xx,yy,zz;

    boundingBox[0]=boundingBox[2]=boundingBox[4]=1.0e+32;
    boundingBox[1]=boundingBox[3]=boundingBox[5]=-1.0e+32;

    for(PosMesh=meshList.GetHeadPosition();PosMesh!=NULL;) {
        QMeshPatch *mesh=(QMeshPatch *)(meshList.GetNext(PosMesh));
        for(Pos=mesh->GetNodeList().GetHeadPosition();Pos!=NULL;) {
            QMeshNode *node=(QMeshNode *)(mesh->GetNodeList().GetNext(Pos));
            node->GetCoord3D(xx,yy,zz);

            if (xx<boundingBox[0]) boundingBox[0]=xx;
            if (xx>boundingBox[1]) boundingBox[1]=xx;
            if (yy<boundingBox[2]) boundingBox[2]=yy;
            if (yy>boundingBox[3]) boundingBox[3]=yy;
            if (zz<boundingBox[4]) boundingBox[4]=zz;
            if (zz>boundingBox[5]) boundingBox[5]=zz;
        }
    }
}

void PolygenMesh::DeleteGLList()
{
    if (m_drawListID!=-1) {
        glDeleteLists(m_drawListID, m_drawListNumber);
        m_drawListID=-1;
    }
}

void PolygenMesh::BuildGLList(bool bVertexNormalShading)
{
    if (m_drawListID!=-1) glDeleteLists(m_drawListID, m_drawListNumber);
    m_drawListID = glGenLists(m_drawListNumber);

    _buildDrawShadeList(bVertexNormalShading);
    _buildDrawMeshList();
    _buildDrawNodeList();
    _buildDrawProfileList();
    _buildDrawFaceNormalList();
    _buildDrawNodeNormalList();
    computeRange();
}

void PolygenMesh::_buildDrawShadeList(bool bVertexNormalShading)
{
    GLKPOSITION Pos;
    GLKPOSITION PosFace;
    GLKPOSITION PosNode;
    QMeshFace* face;
    QMeshNode* node;
    QMeshPatch* mesh;
    double xx, yy, zz, dd;		float rr, gg, bb;
    int k, i, num, meshIndex;
    glNewList(m_drawListID, GL_COMPILE);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

	drawOriginalCoordinate();
    if (isTransparent) {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    meshIndex = 0;
    for (Pos = meshList.GetHeadPosition(); Pos != NULL; meshIndex++) {
        mesh = (QMeshPatch*)(meshList.GetNext(Pos));
        glBegin(GL_TRIANGLES);
        for (PosFace = (mesh->GetFaceList()).GetHeadPosition(); PosFace != NULL;)
        {
            face = (QMeshFace*)((mesh->GetFaceList()).GetNext(PosFace));

            //These are the normal case
			if (mesh->isVolume == true && face->inner == true) continue;
            if (face->m_nIdentifiedPatchIndex < 0) {
                if (meshIndex == 0) { rr = gg = bb = 0.8f; }
                else if (meshIndex == 1) { rr = gg = 0.8f; bb = 0.5f; }
                else { rr = gg = bb = 0.8f; }
                glColor3f(rr, gg, bb);
                if (face->GetAttribFlag(4)) glColor3f(1.0f, 1.0f, 0.0f);
                if (face->GetAttribFlag(5)) glColor3f(0.0f, 0.0f, 0.0f);
                if (face->selected)         glColor3f(0.6, 0.6, 1.0);
                //else if (face->isFixedDraw)   glColor3f(0.3, 0.8, 1.0);
                //else if (face->isHandleDraw)  glColor3f(0.8, 0.3, 1.0);
               
            }
            else {
                _changeValueToColor(face->m_nIdentifiedPatchIndex, rr, gg, bb);
                glColor3f(rr, gg, bb);
            }

            if (isTransparent)  glColor4f(rr, gg, bb, 0.8);
            

            num = face->GetEdgeNum();
            for (k = 0; k < num - 2; k++) {
                for (i = 0; i < 3; i++) {
                    if (i < 1)
                        node = face->GetNodeRecordPtr(i);
                    else
                        node = face->GetNodeRecordPtr(i + k);
                    bVertexNormalShading = false;
                    if (bVertexNormalShading) { // && face->m_nIdentifiedPatchIndex>0) {
                        double normal[3];
                        node->CalNormal(normal);
                        glNormal3dv(normal);
                    }
                    else {
                        face->GetPlaneEquation(xx, yy, zz, dd);
                        glNormal3d(xx, yy, zz);
                    }
                    node->GetCoord3D(xx, yy, zz);
                    glVertex3d(xx, yy, zz);
                }
            }
        }
        glEnd();
    }

    if (isTransparent) {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }

    glEndList();
}

void PolygenMesh::_changeValueToColor(int nType, float & nRed, float & nGreen, float & nBlue)
{
    float color[][3]={
        {220,20,60},
        {107,200,35},
        {30,144,255},
        {255,105,180},
        {244,164,96},
        {176,196,222},
        {255,100,70},
        {128,255,128},
        {128,128,255},
        {255,255,128},
        {0,128,0},
        {255,128,255},
        {255,214,202},
        {128,128,192},
        {255,165,0}, //orange
        {255,128,192},
//		{39, 64, 139},//RoyalBlue
        {128,128,64},
        {0,255,255},
        {238,130,238},//violet
        {220,220,220},//gainsboro
        {188, 143, 143}, // rosy brown
        {46, 139, 87},//sea green
        {210, 105, 30 },//chocolate
        {237, 150, 100},
        {100, 149, 237},//cornflower blue
        {243, 20, 100},
        // 26th
        {0,0,0}
    };

//	printf("%d ",nType);
    nRed=color[nType%25][0]/255.0f;
    nGreen=color[nType%25][1]/255.0f;
    nBlue=color[nType%25][2]/255.0f;
}

void PolygenMesh::_buildDrawMeshList()
{
    GLKPOSITION Pos;
    GLKPOSITION PosEdge;
    float rr, gg, bb;

    if (meshList.GetCount()==0) return;

    glNewList(m_drawListID+1, GL_COMPILE);
    glDisable(GL_LIGHTING);
    // glLineWidth(0.7);
    if (edgeColor)  glLineWidth(1.0);
    glLineWidth(1.0);
    QMeshEdge* edge;
    QMeshPatch* mesh;
    double xx, yy, zz;

    glBegin(GL_LINES);
    int meshIndex = 0;
    for (Pos = meshList.GetHeadPosition(); Pos != NULL; meshIndex++) {
        mesh = (QMeshPatch*)(meshList.GetNext(Pos));
        for (PosEdge = (mesh->GetEdgeList()).GetHeadPosition(); PosEdge != NULL;) {
            edge = (QMeshEdge*)((mesh->GetEdgeList()).GetNext(PosEdge));

            rr = 0.2; gg = 0.2; bb = 0.2;
            glColor3f(rr, gg, bb);

            //if (edge->cableIndex >= 0)  glColor3f(0.9, 0.9, 0.9);
            //if (edge->selected)         glColor3f(0.6, 0.6, 1.0);

            edge->GetStartPoint()->GetCoord3D(xx, yy, zz);
            glVertex3d(xx, yy, zz);
            edge->GetEndPoint()->GetCoord3D(xx, yy, zz);
            glVertex3d(xx, yy, zz);
        }
    }
    glEnd();
    glEndList();
}

void PolygenMesh::_buildDrawNodeList()
{
    GLKPOSITION Pos;
    GLKPOSITION PosNode;
    float rr, gg, bb;

    if (meshList.GetCount()==0) return;

    glNewList(m_drawListID+2, GL_COMPILE);
    glDisable(GL_LIGHTING);

    QMeshNode *node;
    QMeshPatch *mesh;
    double xx,yy,zz,nx,ny,nz;

    glEnable(GL_POINT_SMOOTH);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // glPointSize(4.0);

    glPointSize(4.0);
    glBegin(GL_POINTS);// plot points
    int meshIndex = 0;
    for (Pos = meshList.GetHeadPosition(); Pos != NULL; meshIndex++) {
        mesh = (QMeshPatch*)(meshList.GetNext(Pos));

        for (PosNode = (mesh->GetNodeList()).GetHeadPosition(); PosNode != NULL;) {
            node = (QMeshNode*)((mesh->GetNodeList()).GetNext(PosNode));

            rr = 0.75; gg = 0.75; bb = 0.75;
            if (node->selected) {
                rr = 0.6; gg = 0.6; bb = 1.0;
            }
            else if (node->isFixed) {
                rr = 0.3; gg = 0.8; bb = 1.0;
            }
            else if (node->isHandle) {
                rr = 0.8; gg = 0.3; bb = 1.0;
            }
            else {}

            if (node->m_nIdentifiedPatchIndex >= 0)
                _changeValueToColor(node->m_nIdentifiedPatchIndex, rr, gg, bb);


            if (mesh->isoSurfaceGuideFieldComputed)
                //_changeValueToColor(1, 0, node->zigzagValue, rr, gg, bb);
                _changeValueToColor(1, 0, node->geoFieldValue, rr, gg, bb);

            
            glColor3f(rr, gg, bb);
            node->SetColor(rr, gg, bb);

            node->GetNormal(nx, ny, nz);
            node->GetCoord3D(xx, yy, zz);
            glNormal3d(nx, ny, nz);
            glVertex3d(xx, yy, zz);
        }
    }
    glEnd();
    glEndList();
}

void PolygenMesh::_buildDrawProfileList()
{
//    GLKPOSITION Pos;
//    GLKPOSITION PosEdge;
//    double xx,yy,zz;

//    if (meshList.GetCount()==0) return;

//    glNewList(m_drawListID+3, GL_COMPILE);
}

void PolygenMesh::_buildDrawFaceNormalList()
{
    if (this->meshType == Tool_PATH) return;
    if (meshList.GetCount()==0) return;

    glNewList(m_drawListID+4, GL_COMPILE);
    glDisable(GL_LIGHTING);

    glColor3f(0.5, 0.0, 0.5);

    glLineWidth(1.0);


    if (this->meshType == TET) {

        glBegin(GL_LINES);
        for (GLKPOSITION meshPos = meshList.GetHeadPosition(); meshPos != NULL;) {
            QMeshPatch* mesh = (QMeshPatch*)(meshList.GetNext(meshPos));
            QMeshEdge* edge = (QMeshEdge*)mesh->GetEdgeList().GetHead();
            double length = edge->CalLength();
            //std::cout << "Face normal length is: " << length << std::endl;
            for (GLKPOSITION Pos = mesh->GetFaceList().GetHeadPosition(); Pos != NULL;) {
                QMeshFace* face = (QMeshFace*)(mesh->GetFaceList().GetNext(Pos));

                // only show the surface normals
                if (face->GetLeftTetra() != NULL && face->GetRightTetra() != NULL) continue;

                double x, y, z, nx, ny, nz;
                face->CalCenterPos(x, y, z);
                face->CalPlaneEquation();
                face->GetNormal(nx, ny, nz);
                glVertex3d(x, y, z);
                glVertex3d(x + nx * length, y + ny * length, z + nz * length);
            }
        }
        glEnd();

    }
    else {
        glBegin(GL_LINES);
        for (GLKPOSITION meshPos = meshList.GetHeadPosition(); meshPos != NULL;) {
            QMeshPatch* mesh = (QMeshPatch*)(meshList.GetNext(meshPos));
            QMeshEdge* edge = (QMeshEdge*)mesh->GetEdgeList().GetHead();
            double length = edge->CalLength();
            for (GLKPOSITION Pos = mesh->GetFaceList().GetHeadPosition(); Pos != NULL;) {
                QMeshFace* face = (QMeshFace*)(mesh->GetFaceList().GetNext(Pos));
                double x, y, z, nx, ny, nz;
                face->CalCenterPos(x, y, z);
                face->CalPlaneEquation();
                face->GetNormal(nx, ny, nz);
                glVertex3d(x, y, z);
                glVertex3d(x + nx * length, y + ny * length, z + nz * length);
            }
        }
        glEnd();
    }

    glEndList();
}

void PolygenMesh::_buildDrawNodeNormalList()
{
    if (meshList.GetCount()==0) return;

    glNewList(m_drawListID+5, GL_COMPILE);
    glDisable(GL_LIGHTING);

    glColor3f(0.0, 0.5, 0.0);

    glLineWidth(1.0);

    glBegin(GL_LINES);
    for (GLKPOSITION meshPos = meshList.GetHeadPosition(); meshPos != NULL;) {
        QMeshPatch* mesh = (QMeshPatch*)(meshList.GetNext(meshPos));
        double length;
        if (this->meshType == Tool_PATH) length = 1.0;
        else {
            QMeshEdge* edge = (QMeshEdge*)mesh->GetEdgeList().GetHead();
            length = edge->CalLength();
        }
        
        //double length = 1.0;
        for (GLKPOSITION Pos = mesh->GetNodeList().GetHeadPosition(); Pos != NULL;) {
            QMeshNode* node = (QMeshNode*)(mesh->GetNodeList().GetNext(Pos));
            double x, y, z;
            node->GetCoord3D(x, y, z);
            double n[3];
            if (this->meshType == Tool_PATH) node->GetNormal(n[0], n[1], n[2]);
            else node->CalNormal(n);

            glVertex3d(x, y, z);
            glVertex3d(x + n[0] * length, y + n[1] * length, z + n[2] * length);
        }
    }
    glEnd();
    
    glEndList();
}

void PolygenMesh::drawOriginalCoordinate() {
	double axisLeng = 30.0;
	glLineWidth(2.0);
	glBegin(GL_LINES);

	// X-axis - Red Color
	glColor3f(1.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(axisLeng, 0.0, 0.0);

	// Y-axis - green Color
	glColor3f(0.0, 1.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, axisLeng, 0.0);

	// Z-axis - black Color
	glColor3f(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, axisLeng);

	//glColor3f(1, 0.1, 0.1);glVertex3d(0.0, 0.0, 0.0);
	//glVertex3d(-7.72805,- 11.4536,- 3.48036); //voxel searching debug

	glEnd();

	
}

void PolygenMesh::drawShade()
{
    if (meshList.IsEmpty()) {glDeleteLists(m_drawListID, m_drawListNumber); m_drawListID=-1; return;}
    glCallList(m_drawListID);
}

void PolygenMesh::drawMesh()
{
    if (meshList.IsEmpty()) {glDeleteLists(m_drawListID, m_drawListNumber); m_drawListID=-1; return;}
    glCallList(m_drawListID+1);
}

void PolygenMesh::drawNode()
{
    if (meshList.IsEmpty()) {glDeleteLists(m_drawListID, m_drawListNumber); m_drawListID=-1; return;}
    glCallList(m_drawListID+2);
}

void PolygenMesh::drawProfile()
{
    if (meshList.IsEmpty()) {glDeleteLists(m_drawListID, m_drawListNumber); m_drawListID=-1; return;}
    glCallList(m_drawListID+3);
}

void PolygenMesh::drawFaceNormal()
{
    if (meshList.IsEmpty()) {glDeleteLists(m_drawListID, m_drawListNumber); m_drawListID=-1; return;}
    glCallList(m_drawListID+4);
}

void PolygenMesh::drawNodeNormal()
{
    if (meshList.IsEmpty()) {glDeleteLists(m_drawListID, m_drawListNumber); m_drawListID=-1; return;}
    glCallList(m_drawListID+5);
}

void PolygenMesh::ClearAll()
{
    GLKPOSITION Pos;

    for(Pos=meshList.GetHeadPosition();Pos!=NULL;) {
        QMeshPatch *mesh=(QMeshPatch *)(meshList.GetNext(Pos));
        mesh->ClearAll();
        delete mesh;
    }
    meshList.RemoveAll();
}

void PolygenMesh::computeRange()
{
    double range=0.0,ll,xx,yy,zz;
    GLKPOSITION Pos;
    GLKPOSITION PosNode;

    for(Pos=meshList.GetHeadPosition();Pos!=NULL;) {
        QMeshPatch *mesh=(QMeshPatch *)(meshList.GetNext(Pos));
        for(PosNode=(mesh->GetNodeList()).GetHeadPosition();PosNode!=NULL;) {
            QMeshNode *node=(QMeshNode *)((mesh->GetNodeList()).GetNext(PosNode));

            node->GetCoord3D(xx,yy,zz);
            ll=xx*xx+yy*yy+zz*zz;

            if (ll>range) range=ll;
        }
    }

    m_range=(float)(sqrt(range));
}

void PolygenMesh::_changeValueToColor(double maxValue, double minValue, double Value,
                                 float & nRed, float & nGreen, float & nBlue)
{
//	Value=fabs(Value);

    if (Value<=minValue)
    {
        nRed=0.0;
        nGreen=0.0;
        nBlue=0.0;
        return;
    }

    if ((maxValue-minValue)<0.000000000001)
    {
        nRed=0.0;
        nGreen=0.0;
        nBlue=1.0;
        return;
    }

    double temp=(Value-minValue)/(maxValue-minValue);

//    nRed=(float)(1.0-temp);	nGreen=(float)(1.0-temp); nBlue=(float)(1.0-temp);	return;

    if (temp>0.75)
    {
        nRed=1;
        nGreen=(float)(1.0-(temp-0.75)/0.25);
        if (nGreen<0) nGreen=0.0f;
        nBlue=0;
        return;
    }
    if (temp>0.5)
    {
        nRed=(float)((temp-0.5)/0.25);
        nGreen=1;
        nBlue=0;
        return;
    }
    if (temp>0.25)
    {
        nRed=0;
        nGreen=1;
        nBlue=(float)(1.0-(temp-0.25)/0.25);
        return;
    }
    else
    {
        nRed=0;
        nGreen=(float)(temp/0.25);
        nBlue=1;
    }

//    double t1,t2,t3;
//    t1=0.75;
//    t2=0.5;
//    t3=0.25;
//    if (temp>t1)
//    {
//        nRed=1;
//        nGreen=0.8-(float)(temp-t1)/(1-t1)*0.42;
//        if (nGreen<0.38) nGreen=0.38f;
//        nBlue=0.62-(float)(temp-t1)/(1-t1)*0.4;
//        if (nBlue<0.22) nBlue=0.22;
//        return;
//    }
//    if (temp>t2)
//    {
//        nRed=1;
//        nGreen=1.0-(float)(temp-t2)/(t1-t2)*0.2;
//        if (nGreen<0.8) nGreen=0.8f;
//        nBlue=0.75-(float)(temp-t2)/(t1-t2)*0.13;
//        if (nBlue<0.62) nBlue=0.62f;
//        return;
//    }
//    if (temp>t3)
//    {
//        nRed=(float)(temp-t3)/(t2-t3)*0.31+0.69;
//        if (nRed>1.0) nRed=1.0f;
//        nGreen=(float)(temp-t3)/(t2-t3)*0.09+0.91;
//        if (nGreen>1.0) nGreen=1.0f;
//        nBlue=0.95-(float)(temp-t3)/(t2-t3)*0.2;
//        if (nBlue<0.75) nBlue=0.75f;
//        return;
//    }
//    else
//    {
//        nRed=(float)temp/t3*0.47+0.22;
//        if (nRed>0.69) nRed=0.69f;
//        nGreen=(float)temp/t3*0.53+0.38;
//        if (nGreen>0.91) nGreen=0.91f;
//        nBlue=1.0-(float)temp/t3*0.05;
//        if (nBlue<0.95) nBlue=0.95f;
//        return;
//    }
}

void PolygenMesh::ImportOBJFile(char *filename, std::string modelName)
{
    QMeshPatch *newMesh = new QMeshPatch;
    if (newMesh->inputOBJFile(filename)){
        meshList.AddTail(newMesh);
        computeRange();
        setModelName(modelName);
    }
    else
        delete newMesh;
}

void PolygenMesh::ImportTETFile(char *filename, std::string modelName)
{
	QMeshPatch *newMesh = new QMeshPatch;
	if (newMesh->inputTETFile(filename, false)) {
		meshList.AddTail(newMesh);
		computeRange();
		setModelName(modelName);
	}
	else
		delete newMesh;
}

void PolygenMesh::drawSingleEdge(QMeshEdge* edge) {

    double xx, yy, zz;
    edge->GetStartPoint()->GetCoord3D(xx, yy, zz);
    glVertex3d(xx, yy, zz);
    edge->GetEndPoint()->GetCoord3D(xx, yy, zz);
    glVertex3d(xx, yy, zz);

}

void PolygenMesh::drawSingleNode(QMeshNode* node) {

    double nx, ny, nz, xx, yy, zz;
    node->GetNormal(nx, ny, nz);
    node->GetCoord3D(xx, yy, zz);
    glNormal3d(nx, ny, nz);
    glVertex3d(xx, yy, zz);

}

void PolygenMesh::drawSingleFace(QMeshFace* face) {

    double xx, yy, zz, dd;
    for (int i = 0; i < 3; i++) {
        QMeshNode* node = face->GetNodeRecordPtr(i);

        if (m_bVertexNormalShading) {
            double normal[3];
            node->CalNormal(normal); glNormal3dv(normal);
        }
        else {
            face->CalPlaneEquation();
            face->GetPlaneEquation(xx, yy, zz, dd);
            glNormal3d(xx, yy, zz);
        }

        node->GetCoord3D(xx, yy, zz);
        glVertex3d(xx, yy, zz);
    }
}