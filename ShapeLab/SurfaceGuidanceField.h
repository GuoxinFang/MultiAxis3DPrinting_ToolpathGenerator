#pragma once
#include "..\QMeshLib\PolygenMesh.h"
#include "..\ThirdPartyDependence\Eigen\Eigen"

class SurfaceGuidanceField
{
public:
	SurfaceGuidanceField() { };
	~SurfaceGuidanceField() { };

	void runIsoLayerVectorFielCompute(QMeshPatch* isoSurface);
	bool scalarFieldCompute_isoSurface(QMeshPatch* layer);
	void scalarFieldCompute_supportSurface(QMeshPatch* layer);

	Eigen::Vector3d inputDir;

private:
	bool _initializeIsoSurface(QMeshPatch* isoSurface);
	bool _SurfaceVectorFieldOrientationDetection(QMeshPatch* isoSurface);
	void _projectVectortoSurface(QMeshFace* Face);

	void _surfaceVectorFieldFulfill(QMeshPatch* isoSurface, int iter, int type);
	void _surfaceVectorFieldFillNIERegion(QMeshPatch* isoSurface);

};
