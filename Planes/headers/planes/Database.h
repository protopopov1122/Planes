/*
  MIT License

  Copyright (c) 2018 Jevgēnijs Protopopovs

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef PLANES_DATABASE_H_
#define PLANES_DATABASE_H_

#include <sqlite3.h>
#include <chrono>
#include "planes/PlaneSession.h"

namespace Planes {

  struct SessionDTO {
    int64_t id;
    time_t opened;
    time_t closed;
  };

  struct PlaneDTO {
    int64_t id;
    int64_t session_id;
    std::string name;
    std::string model;
    time_t created;
    time_t destroyed;
  };

  class PlaneDatabase {
    public:
      PlaneDatabase(std::string);
      ~PlaneDatabase();
      int64_t openSession(PlaneSession &);
      bool closeSession(int64_t);
      int64_t newPlane(int64_t, Plane &);
      bool destroyPlane(int64_t);

      void getSessions(std::vector<SessionDTO> &);
      void getPlanes(int64_t, std::vector<PlaneDTO> &);
    private:
      void prepareTables();
      static int sessionGetter(void *, int, char **, char **);
      static int planeGetter(void *, int, char **, char **);
      
      sqlite3 *db;
  };
}

#endif