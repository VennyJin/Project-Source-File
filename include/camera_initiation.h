
#include "spinnaker/Spinnaker.h"
#include <spinnaker/SpinGenApi/SpinnakerGenApi.h>
#include <iostream>
#include <sstream>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

int AcquireImages(CameraPtr pCam, INodeMap& nodeMap, INodeMap& nodeMapTLDevice);
int PrintDeviceInfo(INodeMap& nodeMap);
int RunSingleCamera(CameraPtr pCam);
int InitializateCamera(void);