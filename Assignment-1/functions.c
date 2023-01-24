#include "functions.h"

char* func_root(void* _arg) {
  if (_arg != NULL)
  {
    char* result = "Hello, Query parameter was not expected for this function!!";
    return result;
  }
  else{
    char* result = "Hello World!!";
    return result;
  }
}

char* func_square(void* _arg) {
  long result = 1;
  char str[25];
  if (_arg != NULL)
  {
    int len = mg_get_var2((char*)_arg, strlen((char*)_arg), "num", str, sizeof(str), 0);
    if (len <= 0)
    {
      char* out = "Hello, num was not found in the query!!";
      return out;
    }
    else{
      char *endptr;
      long n = strtol(str, &endptr, 10);
      if (errno == ERANGE || *endptr != '\0') {
        char* out = "Hello, value of num is not valid!!";
        return out;
      }
      result = n * n ;
    }
  }

  sprintf(str, "%ld", result);

  char* res = str;
  return res;
}

char* func_cube(void* _arg) {
  long result = 1;
  char str[25];
  if (_arg != NULL)
  {
    int len = mg_get_var2((char*)_arg, strlen((char*)_arg), "num", str, sizeof(str), 0);
    if (len <= 0)
    {
      char* out = "Hello, num was not found in the query!!";
      return out;
    }
    else{
      char *endptr;
      long n = strtol(str, &endptr, 10);
      if (errno == ERANGE || *endptr != '\0') {
        char* out = "Hello, value of num is not valid!!";
        return out;
      }
      result = n * n * n;
    }
  }

  sprintf(str, "%ld", result);

  char* res = str;
  return res;
}

char* func_helloworld(void* _arg) {
  char* result = "Hello";
  char str[100];
  if (_arg != NULL)
  {
    int len = mg_get_var2((char*)_arg, strlen((char*)_arg), "str", str, sizeof(str), 0);
    if (len <= 0)
    {
      char* out = "Hello, str was not found in the query!!";
      return out;
    }
    char str1[] = "Hello, ";
    int total_len = strlen(str1) + len + 1;

    char res[100 + strlen(str1) + 1];
    
    snprintf(res, total_len + 1, "%s%s", str1, str);
    result = res;
  }
  return result;
}

char* func_prime(void* _arg) {
  char* result = "False";
  char str[25];
  if (_arg != NULL)
  {
    int len = mg_get_var2((char*)_arg, strlen((char*)_arg), "num", str, sizeof(str), 0);
    if (len <= 0)
    {
      char* out = "Hello, num was not found in the query!!";
      return out;
    }
    char *endptr;
    int arg = strtol(str, &endptr, 10);
    if (errno == ERANGE || *endptr != '\0') {
      char* out = "Hello, value of num is not valid!!";
      return out;
    }
    if (arg == 2)
    {
      result = "True";
    }
    else if (arg % 2 == 0 || arg <= 1)
    {
      result = "False";
    }
    else
    {
      int i;
      for (i = 3; i * i <= arg; i += 2)
      {
        if (arg % i == 0)
        {
          result = "False";
          break;
        }
      }
      if (i * i > arg)
      {
        result = "True";
      }
    }
  }
  return result;
}


char* func_pingpong(void* _arg) {
  char* result = "PingPong";
  if (_arg != NULL)
  {
    char str[100];
    int len = mg_get_var2((char*)_arg, strlen((char*)_arg), "str", str, sizeof(str), 0);
    if (len <= 0)
    {
      char* resu = "Hello, str was not found in the query!!";
      return resu;
    }
    result = str;
  }
  return result;
}

char* func_fibonacci(void* _arg) {

  long result = 1;
  if (_arg != NULL)
  {
    char str[25];
    int len = mg_get_var2((char*)_arg, strlen((char*)_arg), "num", str, sizeof(str), 0);
    if (len <= 0)
    {
      char* out= "Hello, num was not found in the query!!";
      return out;
    }
    char *endptr;
    long n = strtol(str, &endptr, 10);
    if (errno == ERANGE || *endptr != '\0') {
      char* out = "Hello, value of num is not valid!!";
      return out;
    }
    long arg = n;
    long a = 0, b =1;
    for (long i = 2; i <= arg; i++)
    {
      long c = a + b;
      a = b;
      b = c;
    }
    result = b;
  }
  char str2[25];

  sprintf(str2, "%ld", result);

   char* res = str2;
  return res;
}

int get_index(const char* url) {
    for (int i = 0; i < NUM_URLS; i++) {
        if (strcmp(urls[i], url) == 0) {
            return i;
        }
    }
    return -1;  // url not found
}