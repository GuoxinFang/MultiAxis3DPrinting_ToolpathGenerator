#include "stdafx.h"
#include "SurfaceGuidanceField.h"

using namespace std;
using namespace Eigen;

void SurfaceGuidanceField::runIsoLayerVectorFielCompute(QMeshPatch* isoSurface) {

	/* Compute the vector field of each iso-surface */

	// project vector field to the plane defined by triangle face
	for (GLKPOSITION Pos = isoSurface->GetFaceList().GetHeadPosition(); Pos;) {
		QMeshFace* Face = (QMeshFace*)isoSurface->GetFaceList().GetNext(Pos);
		Face->principleStressDir = this->inputDir.normalized();
		this->_projectVectortoSurface(Face);
	}

}

void SurfaceGuidanceField::_projectVectortoSurface(QMeshFace* Face) {
	Eigen::Vector3d faceNorm; double D;
	Face->CalPlaneEquation(); Face->GetPlaneEquation(faceNorm(0), faceNorm(1), faceNorm(2), D);
	faceNorm = faceNorm.normalized();
	double dotProduct = Face->principleStressDir.dot(faceNorm);
	Eigen::Vector3d planeNorm = Face->principleStressDir - dotProduct * faceNorm;
	//Face->principleStressDir = planeNorm.normalized().cross(faceNorm);
	Face->principleStressDir = planeNorm.normalized();
}

bool SurfaceGuidanceField::scalarFieldCompute_isoSurface(QMeshPatch* isoSurface) {

	int index = 0;
	for (GLKPOSITION Pos = isoSurface->GetNodeList().GetHeadPosition(); Pos;) {
		QMeshNode* Node = (QMeshNode*)isoSurface->GetNodeList().GetNext(Pos);
		Node->SetIndexNo(index); index++;
	}

	// ---- method 1: apply laplacian to all the element (including constrained)

	Eigen::SparseMatrix<double> Parameter(3 * isoSurface->GetFaceNumber(), isoSurface->GetNodeNumber()); //A

	Eigen::VectorXd guideField(isoSurface->GetNodeNumber()); //x

	Eigen::VectorXd b(3 * isoSurface->GetFaceNumber()); //b
	b.setZero();

	std::vector<Eigen::Triplet<double>> ParaTriplet;

	index = 0;
	for (GLKPOSITION Pos = isoSurface->GetFaceList().GetHeadPosition(); Pos;) {
		QMeshFace* Face = (QMeshFace*)isoSurface->GetFaceList().GetNext(Pos);
		Face->SetIndexNo(index); index++;
		double weight = 1.0;
		//if (Face->isNIEface == false) weight = 3.0;
		double faceAera = Face->CalArea();

		Eigen::Matrix3d faceMatrixPara, faceNodeCoord;
		for (int i = 0; i < 3; i++) {
			QMeshNode* Node = Face->GetNodeRecordPtr(i);
			Node->GetCoord3D(faceNodeCoord(i, 0), faceNodeCoord(i, 1), faceNodeCoord(i, 2));
		}
		faceMatrixPara.row(0) = (faceNodeCoord.row(2) - faceNodeCoord.row(1)) / (2 * faceAera);
		faceMatrixPara.row(1) = (faceNodeCoord.row(0) - faceNodeCoord.row(2)) / (2 * faceAera);
		faceMatrixPara.row(2) = (faceNodeCoord.row(1) - faceNodeCoord.row(0)) / (2 * faceAera);

		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 3; i++) {
				QMeshNode* Node = Face->GetNodeRecordPtr(i);
				ParaTriplet.push_back(Eigen::Triplet<double>(
					Face->GetIndexNo() * 3 + j, Node->GetIndexNo(), faceMatrixPara(i, j) * weight)); // infill A
			}
		}

		for (int i = 0; i < 3; i++) b(Face->GetIndexNo() * 3 + i) = Face->principleStressDir(i) * weight; // infill B
	}

	Parameter.setFromTriplets(ParaTriplet.begin(), ParaTriplet.end());

	Eigen::SparseMatrix<double> ATA(isoSurface->GetNodeNumber(), isoSurface->GetNodeNumber());
	ATA = Parameter.transpose() * Parameter;
	Eigen::SparseLU <Eigen::SparseMatrix<double>> Solver;

	//Solver.compute(ATA);
	Solver.analyzePattern(ATA);
	Solver.factorize(ATA);
	if (Solver.info() != Eigen::Success) { 
		cout << "this layer has error computing scalar field !" << endl; return false; 
	}

	Eigen::VectorXd ATb(isoSurface->GetNodeNumber());
	ATb = Parameter.transpose() * b;
	guideField = Solver.solve(ATb);

	Eigen::VectorXd guideFieldNormalize(isoSurface->GetNodeNumber());
	// compute max and min phis
	double minPhi = INFINITY;
	double maxPhi = -INFINITY;

	for (int i = 0; i < isoSurface->GetNodeNumber(); i++) {
		if (minPhi > guideField(i)) minPhi = guideField(i);
		if (maxPhi < guideField(i)) maxPhi = guideField(i);
	}
	double range = maxPhi - minPhi;

	for (int i = 0; i < isoSurface->GetNodeNumber(); i++)
		guideFieldNormalize(i) = 1 - (guideField(i) - minPhi) / range;

	for (GLKPOSITION Pos = isoSurface->GetNodeList().GetHeadPosition(); Pos;) {
		QMeshNode* Node = (QMeshNode*)isoSurface->GetNodeList().GetNext(Pos);
		//Node->guideFieldValue = guideField(Node->GetIndexNo());
		Node->guideFieldValue = guideFieldNormalize(Node->GetIndexNo());
		Node->zigzagValue = guideFieldNormalize(Node->GetIndexNo());

	}
	isoSurface->isoSurfaceGuideFieldComputed = true;
	return true;
}

