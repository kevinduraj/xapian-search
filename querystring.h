#ifndef QUERY_STRING_H_
#define QUERY_STRING_H_
/*-------------------------------------------------------------------------------------------*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
/*-------------------------------------------------------------------------------------------*/
// Helper macro to convert two-character hex strings to character value
#define ToHex(Y) (Y>='0'&&Y<='9'?Y-'0':Y-'A'+10)
/*-------------------------------------------------------------------------------------------*/
char InputData[4096]; 
/*-------------------------------------------------------------------------------------------*/
/*                           Initialize All Parameters                                       */
/*-------------------------------------------------------------------------------------------*/
void getAllParams() 
{
  // Determing if it is a POST or GET method
  if( getenv( "REQUEST_METHOD" ) == 0 ) 
  {
    printf("No REQUEST_METHOD\n");
    return;
  } 
  else if (strcmp( getenv("REQUEST_METHOD"), "POST") == 0) 
  {
    char *endptr;	// quite useless, but required 
    char *len1 = getenv("CONTENT_LENGTH");
    int contentlength = strtol(len1, &endptr, 10);
    fread(InputData , contentlength, 1, stdin);
  } 
  else // GET METHOD
  {
    strcpy(InputData, getenv("QUERY_STRING"));
  }
}
/*-------------------------------------------------------------------------------------------*/
/*                                   Get One Parameter                                       */
/*-------------------------------------------------------------------------------------------*/
void getParam(const char *Name, char *Value) 
{ 
  char *pos1 = strstr(InputData, Name); 

  if (pos1) 
  { 
    pos1 += strlen(Name); 
    if (*pos1 == '=') // Make sure there is an '=' where we expect it 
    {
      pos1++; 

      while (*pos1 && *pos1 != '&') 
      { 
        if (*pos1 == '%') 
        { // Convert it to a single ASCII character and store at our Valueination 
          *Value++ = (char)ToHex(pos1[1]) * 16 + ToHex(pos1[2]); 
          pos1 += 3; 
        } 
        else if( *pos1=='+' ) 
        { 
          // If it's a '+', store a space at our Valueination 
          *Value++ = ' '; 
          pos1++; 
        } 
        else 
        { 
          *Value++ = *pos1++; // Otherwise, just store the character at our Valueination 
        } 
      } 
      *Value++ = '\0';
      return; 
    }  
  } 

  strcpy(Value, "null");

  return; 
} 
/*-------------------------------------------------------------------------------------------*/

#endif /* QUERY_STRING_H_ */

