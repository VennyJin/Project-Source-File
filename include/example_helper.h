#ifndef _EXAMPLE_HELPER_H_
#define _EXAMPLE_HELPER_H_

#include <ajile/AJObjects.h>
#include <vector>

int RunExample(aj::Project (*createFunction)(unsigned short, unsigned int, float, std::vector<aj::Component>), int argc, char *argv[]);

#endif
