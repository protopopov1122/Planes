#include "football/ResourceManager.h"

namespace Football {

#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif 

  ResourceManager::ResourceManager(std::string base) : basePath(base) {
	 wxInitAllImageHandlers();
  }

  const std::string &ResourceManager::getBasePath() const {
    return this->basePath;
  }

  std::string ResourceManager::getFullPath(std::string path) const {
    if (this->basePath.empty()) {
      return path;
    } else {
      return this->basePath + PATH_SEPARATOR + path;
    }
  }

  wxBitmap ResourceManager::loadBitmap(std::string path) const {
    std::string fullPath = this->getFullPath(path);
	wxImage img(fullPath, wxBITMAP_TYPE_PNG);
    wxBitmap bitmap(img);
    return bitmap;
  }
}