# Locale
solve a problem of Locale 

## Locate
In C/C + + programs, locale (namely system locale, namely national or regional Settings) will change the current language coding format, date format, digital and other related to the regional Settings. Locale Settings correctly or not will affect the string handling in a program (How to output a wchar_t? What should the format of strftime() be? etc.). Therefore, for each program, the locale setting should be handled with caution.

## A problem
Some times ago, I encountered such a bug that the data I read from the XML file was wrong when I set up the language area of the computer to Sweden . The real reason is that, in order for the software to be displayed correctly, the software has set the locale to Sweden which is the same as the computer settings when it starts. In Swedish, the decimal point is ", "and not“.”, which leads to errors in floating point Numbers that are read from the XML file.

It is necessary to initialize the locate of the software at startup , at the same time it is also indispensable to read some XML files during the use of software. Therefore, if you do not solve this problem, it is likely that the data read from the file is wrong.
``` C++
// set up locale
_tsetlocale(LC_ALL, TEXT(""));
```

## Solution
New idea: Temporarily changed the language to English to make sure that the data I read from the file was correct when I read the file. But the code will be very redundant if you reset the locate every time. The new solution is to write a class in a public place and implement the function of temporarily modifying the locale.
