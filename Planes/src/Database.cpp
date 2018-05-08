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

#include <sstream>
#include <ctime>
#include <wx/msgdlg.h>
#include "planes/Database.h"

namespace Planes {
  PlaneDatabase::PlaneDatabase(std::string dbname) {
    if (sqlite3_open(dbname.c_str(), &this->db) != SQLITE_OK) {
      wxMessageBox("SQLite error: " + std::string(sqlite3_errmsg(this->db)), "Error", wxOK | wxICON_ERROR);
    }
    this->prepareTables();
  }

  PlaneDatabase::~PlaneDatabase() {
    sqlite3_close(this->db);
  }

  int64_t PlaneDatabase::openSession(PlaneSession &sess) {
    auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(sess.getOpenTime().time_since_epoch()).count();
    std::stringstream ss;
    ss << "INSERT INTO Sessions(Opened) VALUES (" << epoch_time << ");";
    char *err = NULL;
    sqlite3_exec(this->db, ss.str().c_str(), nullptr, nullptr, &err);
    if (err != NULL) {
      wxMessageBox("SQLite error: " + std::string(err), "Error", wxOK | wxICON_ERROR);
      return -1;
    } else {
      int64_t sid = sqlite3_last_insert_rowid(this->db);
      return sid;
    }
  }

  bool PlaneDatabase::closeSession(int64_t sid) {
    auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::stringstream ss;
    ss << "UPDATE Sessions SET Closed=" << epoch_time << " WHERE Id=" << sid << ";";
    char *err = NULL;
    sqlite3_exec(this->db, ss.str().c_str(), nullptr, nullptr, &err);
    if (err != NULL) {
      wxMessageBox("SQLite error: " + std::string(err), "Error", wxOK | wxICON_ERROR);
    }
    return err == NULL;
  }

  int64_t PlaneDatabase::newPlane(int64_t sid, Plane &plane) {
    auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(plane.getCreateTime().time_since_epoch()).count();
    const char *QUERY = "INSERT INTO Planes (SessionId, Name, Model, Created) VALUES (?,?,?,?);";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(this->db, QUERY, -1, &stmt, nullptr);
    std::cout << plane.getName() << std::endl;
    sqlite3_bind_int64(stmt, 1, sid);
    sqlite3_bind_text(stmt, 2, plane.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, plane.getModel().getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, epoch_time);
    int errcode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (errcode != SQLITE_DONE) {
      wxMessageBox("SQLite error: " + std::string(sqlite3_errmsg(this->db)), "Error", wxOK | wxICON_ERROR);
      return -1;
    } else {
      int64_t pid = sqlite3_last_insert_rowid(this->db);
      return pid;
    }
  }

  bool PlaneDatabase::destroyPlane(int64_t pid) {
    auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::stringstream ss;
    ss << "UPDATE Planes SET Destroyed=" << epoch_time << " WHERE Id=" << pid << ";";
    char *err = NULL;
    sqlite3_exec(this->db, ss.str().c_str(), nullptr, nullptr, &err);
    if (err != NULL) {
      wxMessageBox("SQLite error: " + std::string(err), "Error", wxOK | wxICON_ERROR);
    }
    return err == NULL;
  }

  void PlaneDatabase::getSessions(std::vector<SessionDTO> &vec) {
    const char *QUERY = "SELECT * FROM Sessions ORDER BY Opened DESC;";
    char *err = NULL;
    sqlite3_exec(this->db, QUERY, PlaneDatabase::sessionGetter, &vec, &err);
    if (err != NULL) {
      wxMessageBox("SQLite error: " + std::string(err), "Error", wxOK | wxICON_ERROR);
    }
  }

  void PlaneDatabase::getPlanes(int64_t sid, std::vector<PlaneDTO> &vec) {
    std::stringstream ss;
    ss << "SELECT * FROM Planes WHERE SessionId=" << sid << " ORDER BY Created ASC;";
    char *err = NULL;
    sqlite3_exec(this->db, ss.str().c_str(), PlaneDatabase::planeGetter, &vec, &err);
    if (err != NULL) {
      wxMessageBox("SQLite error: " + std::string(err), "Error", wxOK | wxICON_ERROR);
    }
  }

  int PlaneDatabase::sessionGetter(void *raw_ptr, int count, char **values, char **columns) {
    std::vector<SessionDTO> *vec = static_cast<std::vector<SessionDTO> *>(raw_ptr);
    SessionDTO dto = {
      std::stoll(values[0]),
      static_cast<time_t>(std::stoul(values[1])),
      values[2] ? static_cast<time_t>(std::stoul(values[2])) : 0
    };
    vec->push_back(dto);
    return 0;
  }

  int PlaneDatabase::planeGetter(void *raw_ptr, int count, char **values, char **columns) {
    std::vector<PlaneDTO> *vec = static_cast<std::vector<PlaneDTO> *>(raw_ptr);
    PlaneDTO dto = {
      std::stoll(values[0]),
      std::stoll(values[1]),
      std::string(values[2]),
      std::string(values[3]),
      static_cast<time_t>(std::stoul(values[4])),
      values[5] ? static_cast<time_t>(std::stoul(values[5])) : 0
    };
    vec->push_back(dto);
    return 0;
  }

  void PlaneDatabase::prepareTables() {
    const char *SESSIONS_TABLE = "CREATE TABLE IF NOT EXISTS Sessions("
                                 "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Opened INTEGER NOT NULL,"
                                 "Closed INTEGER);";
    const char *PLANES_TABLE = "CREATE TABLE IF NOT EXISTS Planes("
                                 "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "SessionId INTEGER NOT NULL,"
                                 "Name TEXT,"
                                 "Model TEXT,"
                                 "Created INTEGER NOT NULL,"
                                 "Destroyed INTEGER,"
                                 "FOREIGN KEY(SessionId) REFERENCES Sessions(Id));";
    sqlite3_exec(this->db, SESSIONS_TABLE, nullptr, nullptr, nullptr);
    sqlite3_exec(this->db, PLANES_TABLE, nullptr, nullptr, nullptr);
  }
}
