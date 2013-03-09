/*
 * =============================================================================
 *
 *       Filename:  sudoku.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/21/2012 05:36:31 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_EXAMPLES_SUDOKU_SUDOKU_H
#define MUDUO_EXAMPLES_SUDOKU_SUDOKU_H

#include <muduo/base/Types.h>

muduo::string solveSudoku(const muduo::string& puzzle);
const int kCells = 81;

#endif
