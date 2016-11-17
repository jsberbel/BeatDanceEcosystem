/* ========================================================================
   File: BaseState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BaseState.h"

void BaseState::SetResourcesFilePath(const char* ResourcesPathAndFile)
{
	resourcesFolderPath = PathUtils::GetResourcePath();
	resourcesFilePath = resourcesFolderPath.append(ResourcesPathAndFile).c_str();
}