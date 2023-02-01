#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){

vector<vector<long>> red(im.height(), vector<long>(im.width()));
vector<vector<long>> green(im.height(), vector<long>(im.width()));
vector<vector<long>> blue(im.height(), vector<long>(im.width()));

for (unsigned int y = 0; y < im.height(); y++) {
    for (unsigned int x = 0; x < im.width(); x++) {
        RGBAPixel *curr = im.getPixel(x, y);
        red[y][x] = curr->r;
        green[y][x] = curr->g;
        blue[y][x] = curr->b;
    }
}

vector<vector<long>> tempSumRed(im.height(), vector<long>(im.width()));
vector<vector<long>> tempSumGreen(im.height(), vector<long>(im.width()));
vector<vector<long>> tempSumBlue(im.height(), vector<long>(im.width()));
vector<vector<long>> tempSumsqRed(im.height(), vector<long>(im.width()));
vector<vector<long>> tempSumsqGreen(im.height(), vector<long>(im.width()));
vector<vector<long>> tempSumsqBlue(im.height(), vector<long>(im.width()));

sumRed = tempSumRed;
sumGreen = tempSumGreen;
sumBlue = tempSumBlue;
sumsqRed = tempSumsqRed;
sumsqGreen = tempSumsqGreen;
sumsqBlue = tempSumsqBlue;

sumRed[0][0] = red[0][0];
sumGreen[0][0] = green[0][0];
sumBlue[0][0] = blue[0][0];

sumsqRed[0][0] = red[0][0] * red[0][0];
sumsqGreen[0][0] = green[0][0] * green[0][0];
sumsqBlue[0][0] = blue[0][0] * blue[0][0];

for (unsigned int y = 1; y < im.height(); y++) {
    sumRed[y][0] = sumRed[y - 1][0] + red[y][0];
    sumGreen[y][0] = sumGreen[y - 1][0] + green[y][0];
    sumBlue[y][0] = sumBlue[y - 1][0] + blue[y][0];

    sumsqRed[y][0] = sumsqRed[y - 1][0] + (red[y][0] * red[y][0]);
    sumsqGreen[y][0] = sumsqGreen[y - 1][0] + (green[y][0] * green[y][0]);
    sumsqBlue[y][0] = sumsqBlue[y - 1][0] + (blue[y][0] * blue[y][0]);

}

for (unsigned int x = 1; x < im.width(); x++) {
    sumRed[0][x] = sumRed[0][x - 1] + red[0][x];
    sumGreen[0][x] = sumGreen[0][x - 1] + green[0][x];
    sumBlue[0][x] = sumBlue[0][x - 1] + blue[0][x];

    sumsqRed[0][x] = sumsqRed[0][x - 1] + (red[0][x] * red[0][x]);
    sumsqGreen[0][x] = sumsqGreen[0][x - 1] + (green[0][x] * green[0][x]);
    sumsqBlue[0][x] = sumsqBlue[0][x - 1] + (blue[0][x] * blue[0][x]);
}

for (unsigned int y = 1; y < im.height(); y++) {
    for (unsigned int x = 1; x < im.width(); x++) {
        sumRed[y][x] = red[y][x] + sumRed[y][x - 1] + sumRed[y - 1][x] - sumRed[y - 1][x - 1];
        sumGreen[y][x] = green[y][x] + sumGreen[y][x - 1] + sumGreen[y - 1][x] - sumGreen[y - 1][x - 1];
        sumBlue[y][x] = blue[y][x] + sumBlue[y][x - 1] + sumBlue[y - 1][x] - sumBlue[y - 1][x - 1];

        sumsqRed[y][x] = (red[y][x] * red[y][x]) + sumsqRed[y][x - 1] + sumsqRed[y - 1][x] - sumsqRed[y - 1][x - 1];
        sumsqGreen[y][x] = (green[y][x] * green[y][x]) + sumsqGreen[y][x - 1] + sumsqGreen[y - 1][x] - sumsqGreen[y - 1][x - 1];
        sumsqBlue[y][x] = (blue[y][x] * blue[y][x]) + sumsqBlue[y][x - 1] + sumsqBlue[y - 1][x] - sumsqBlue[y - 1][x - 1];
    }
}



}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    if (w == 0 || h == 0) {
        return 0;
    }

    long sum = 0;
    if (channel == 'r') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumRed[h - 1][w - 1];
        } else if (ul.first == 0) {
            sum = sumRed[ul.second + h - 1][w - 1] - sumRed[ul.second - 1][w - 1];   
        } else if (ul.second == 0) {
            sum = sumRed[h - 1][ul.first + w - 1] - sumRed[h - 1][ul.first - 1];
        } else {
            sum = sumRed[ul.second + h - 1][ul.first + w - 1] - sumRed[ul.second + h - 1][ul.first - 1] - sumRed[ul.second - 1][ul.first + w - 1] + sumRed[ul.second - 1][ul.first - 1];
        }
        
    } else if (channel == 'g') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumGreen[h - 1][w - 1];
        } else if (ul.first == 0) {
            sum = sumGreen[ul.second + h - 1][w - 1] - sumGreen[ul.second - 1][w - 1];   
        } else if (ul.second == 0) {
            sum = sumGreen[h - 1][ul.first + w - 1] - sumGreen[h - 1][ul.first - 1];
        } else {
            sum = sumGreen[ul.second + h - 1][ul.first + w - 1] - sumGreen[ul.second + h - 1][ul.first - 1] - sumGreen[ul.second - 1][ul.first + w - 1] + sumGreen[ul.second - 1][ul.first - 1];
        }
    } else if (channel == 'b') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumBlue[h - 1][w - 1];
        } else if (ul.first == 0) {
            sum = sumBlue[ul.second + h - 1][w - 1] - sumBlue[ul.second - 1][w - 1];   
        } else if (ul.second == 0) {
            sum = sumBlue[h - 1][ul.first + w - 1] - sumBlue[h - 1][ul.first - 1];
        } else {
            sum = sumBlue[ul.second + h - 1][ul.first + w - 1] - sumBlue[ul.second + h - 1][ul.first - 1] - sumBlue[ul.second - 1][ul.first + w - 1] + sumBlue[ul.second - 1][ul.first - 1];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    if (w == 0 || h == 0) {
        return 0;
    }

    long sum = 0;
    if (channel == 'r') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumsqRed[h - 1][w - 1];
        } else if (ul.first == 0) {
            sum = sumsqRed[ul.second + h - 1][w - 1] - sumsqRed[ul.second - 1][w - 1];   
        } else if (ul.second == 0) {
            sum = sumsqRed[h - 1][ul.first + w - 1] - sumsqRed[h - 1][ul.first - 1];
        } else {
            sum = sumsqRed[ul.second + h - 1][ul.first + w - 1] - sumsqRed[ul.second + h - 1][ul.first - 1] - sumsqRed[ul.second - 1][ul.first + w - 1] + sumsqRed[ul.second - 1][ul.first - 1];
        }
        
    } else if (channel == 'g') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumsqGreen[h - 1][w - 1];
        } else if (ul.first == 0) {
            sum = sumsqGreen[ul.second + h - 1][w - 1] - sumsqGreen[ul.second - 1][w - 1];   
        } else if (ul.second == 0) {
            sum = sumsqGreen[h - 1][ul.first + w - 1] - sumsqGreen[h - 1][ul.first - 1];
        } else {
            sum = sumsqGreen[ul.second + h - 1][ul.first + w - 1] - sumsqGreen[ul.second + h - 1][ul.first - 1] - sumsqGreen[ul.second - 1][ul.first + w - 1] + sumsqGreen[ul.second - 1][ul.first - 1];
        }
    } else if (channel == 'b') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumsqBlue[h - 1][w - 1];
        } else if (ul.first == 0) {
            sum = sumsqBlue[ul.second + h - 1][w - 1] - sumsqBlue[ul.second - 1][w - 1];   
        } else if (ul.second == 0) {
            sum = sumsqBlue[h - 1][ul.first + w - 1] - sumsqBlue[h - 1][ul.first - 1];
        } else {
            sum = sumsqBlue[ul.second + h - 1][ul.first + w - 1] - sumsqBlue[ul.second + h - 1][ul.first - 1] - sumsqBlue[ul.second - 1][ul.first + w - 1] + sumsqBlue[ul.second - 1][ul.first - 1];
        }
    }
    return sum;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double varRed;
    double varGreen;
    double varBlue;
    double area = w * h;

    varRed = (double) getSumSq('r', ul, w, h) - ((double) getSum('r', ul, w, h) * (double) getSum('r', ul, w, h)) / area;

    varGreen = (double) getSumSq('g', ul, w, h) - ((double) getSum('g', ul, w, h) * (double) getSum('g', ul, w, h)) / area;
    varBlue = (double) getSumSq('b', ul, w, h) - ((double) getSum('b', ul, w, h) * (double) getSum('b', ul, w, h)) / area;

    return varRed + varGreen + varBlue;

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    long area = w * h;

    long avgRed = getSum('r', ul, w, h) / area;
    long avgGreen = getSum('g', ul, w, h) / area;
    long avgBlue = getSum('b', ul, w, h) / area;

    return RGBAPixel(avgRed, avgGreen, avgBlue);

}
