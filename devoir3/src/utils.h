#pragma once

namespace Util{
  static char port[] = "10111";
  static const int TIME_WAIT = 10;
  static const int BUF=20;
  static const int BUFFILE=100;
  enum Type{
    BEGIN,
    CONTINUE,
    END
  };
  
  struct msgtrame{
    Type mtype;
    int length;
    char data[BUF];
  };
}

