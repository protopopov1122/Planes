#ifndef FOOTBALL_RESOURCE_MANAGER_H_
#define FOOTBALL_RESOURCE_MANAGER_H_

#include "football/base.h"
#include <wx/bitmap.h>

namespace Football {

class ResourceManager {
public:
  ResourceManager(std::string);
  const std::string &getBasePath() const;
  std::string getFullPath(std::string) const;
  wxBitmap loadBitmap(std::string) const;

private:
  const std::string basePath;
};
}

#endif