#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

#ifndef _FUNC_LOG_
  #ifdef __unix__
    #define _FUNC_LOG_ (log->info("{} {} {}", __PRETTY_FUNCTION__, __LINE__, __FILE__));
  #else
    #define _FUNC_LOG_ (log->info("{}::{}", typeid (*this).name(), __FUNCTION__, __LINE__));
  #endif
#endif

inline std::string str(QString const& str) {
  return str.toStdString();
}

inline std::string str(QJsonObject json) {
  return QString(QJsonDocument(json).toJson()).toStdString();
}

inline std::string str(QJsonDocument const& jsonDoc) {
  return QString(jsonDoc.toJson()).toStdString();
}
#endif // LOG_H
