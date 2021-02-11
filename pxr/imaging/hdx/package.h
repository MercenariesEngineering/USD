//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_IMAGING_HDX_PACKAGE_H
#define PXR_IMAGING_HDX_PACKAGE_H

#include "pxr/pxr.h"
#include "pxr/imaging/hdx/api.h"
#include "pxr/imaging/hdx/version.h"
#include "pxr/base/tf/token.h"

PXR_NAMESPACE_OPEN_SCOPE


HDX_API
TfToken HdxPackageFullscreenShader();
HDX_API
TfToken HdxPackageRenderPassColorShader();
HDX_API
TfToken HdxPackageRenderPassColorAndSelectionShader();
HDX_API
TfToken HdxPackageRenderPassColorWithOccludedSelectionShader();

HDX_API
TfToken HdxPackageRenderPassIdShader();

HDX_API
TfToken HdxPackageRenderPassPickingShader();

HDX_API
TfToken HdxPackageRenderPassShadowShader();

HDX_API
TfToken HdxPackageColorChannelShader();

HDX_API
TfToken HdxPackageColorCorrectionShader();
HDX_API
TfToken HdxPackageVisualizeAovShader();
HDX_API
TfToken HdxPackageRenderPassOitShader();

HDX_API
TfToken HdxPackageRenderPassOitOpaqueShader();

HDX_API
TfToken HdxPackageRenderPassOitVolumeShader();

HDX_API
TfToken HdxPackageOitResolveImageShader();

HDX_API
TfToken HdxPackageOutlineShader();
HDX_API
TfToken HdxPackageSkydomeShader();
HDX_API
TfToken HdxPackageBoundingBoxShader();

HDX_API
TfToken HdxPackageDefaultDomeLightTexture();

PXR_NAMESPACE_CLOSE_SCOPE

#endif
