/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  stats s(imIn);
  pair<int, int> ul(0, 0);
  root = buildTree(s, ul, imIn.width(), imIn.height(), tol);
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node *SQtree::buildTree(stats &s, pair<int, int> &ul,
                                int w, int h, double tol) {
    if (w == 0 || h == 0) {
        return NULL;
    }

    Node *newNode = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));
    if ((w == 1 && h == 1) || s.getVar(ul, w, h) <= tol) {
        return newNode;
    }

    double min = s.getVar(ul, w, h);
    double varNW = 0;
    double varNE = 0;
    double varSW = 0;
    double varSE = 0;
    double x_val, y_val;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (x!=0 && y!= 0)
            varNW = s.getVar({ul.first,     ul.second},     x,     y);
            if (y!=0)
            varNE = s.getVar({ul.first + x, ul.second},     w - x, y);
            if (x!=0)
            varSW = s.getVar({ul.first,     ul.second + y}, x,     h - y);
            if (true)
            varSE = s.getVar({ul.first + x, ul.second + y}, w - x, h - y);

            double max = varNE;
            if (max <= varNW)
            max = varNW;
            if (max <= varSW)
            max = varSW;
            if (max <= varSE)
            max = varSE;

            if (min >= max) {
                min = max;
                x_val = x;
                y_val = y;
            }
        }
    }

    pair<int, int> pairNW = ul;
    pair<int, int> pairNE = {ul.first + x_val, ul.second};
    pair<int, int> pairSW = {ul.first,         ul.second + y_val};
    pair<int, int> pairSE = {ul.first + x_val, ul.second + y_val};

    newNode->NW = buildTree(s, pairNW, x_val,     y_val,     tol);
    newNode->NE = buildTree(s, pairNE, w - x_val, y_val,     tol);
    newNode->SW = buildTree(s, pairSW, x_val,     h - y_val, tol);
    newNode->SE = buildTree(s, pairSE, w - x_val, h - y_val, tol);

    return newNode;
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG img(root->width, root->height);
  render(img, root);
  return img;
}

void SQtree::render(PNG &img, Node* node) {
  if (node == NULL) {
    return;
  }
  
  if (node->NW == NULL && node->NE == NULL && node->SE == NULL && node->SW == NULL) {
    for (int y = 0; y < node->height; y++) {
        for (int x = 0; x < node->width; x++) {
            *(img.getPixel(x + node->upLeft.first, y + node->upLeft.second)) = node->avg;
        }
    }
  } else {
    render(img, node->NW);
    render(img, node->NE);
    render(img, node->SW);
    render(img, node->SE);
  }
}


/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  clear(root);
}

void SQtree::clear(Node *&node) {
  if (!node) {
    return;
  }
  clear(node->NW);
  clear(node->NE);
  clear(node->SE);
  clear(node->SW);
  delete node;
  node = NULL;
}

void SQtree::copy(const SQtree & other) {
  root = copy(other.root);
}

SQtree::Node *SQtree::copy(const Node *other) {
  Node *newNode = new Node(other->upLeft, other->width, other->height, other->avg, other->var);
    if (other->NW != NULL) {
      newNode->NW = copy(other->NW); 
      }
    if (other->NE != NULL) {
      newNode->NE = copy(other->NE); 
      }
    if (other->SE != NULL) {
      newNode->SE = copy(other->SE); 
      }
    if (other->SW != NULL) {
      newNode->SW = copy(other->SW); 
    }

    return newNode;
  }

int SQtree::size() {
  return size(root);
}

int SQtree::size(Node *node) {
  if (!node) {
    return 0;
  }

  return 1 + size(node->NW) + size(node->NE) + size(node->SW) + size(node->SE);
}