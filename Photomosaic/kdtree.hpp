/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> &first,
                                const Point<Dim> &second, int curDim) const {
    /**
     * @todo Implement this function!
     */

    if (first[curDim] != second[curDim]) {
        return first[curDim] < second[curDim];
    } else {
        return first < second;
    }
}

template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> &target,
                                const Point<Dim> &currentBest,
                                const Point<Dim> &potential) const {
    /**
     * @todo Implement this function!
     */
    double currentDistance = 0;
    double potentialDistance = 0;
    for (int i = 0; i < Dim; i++) {
        currentDistance += pow(currentBest[i] - target[i], 2);
    }
    for (int i = 0; i < Dim; i++) {
        potentialDistance += pow(potential[i] - target[i], 2);
    }
    if (potentialDistance == currentDistance) {
        return potential < currentBest;
    }
    return potentialDistance < currentDistance;
}

template<int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>> &list, int left, int right, int pivotIndex, int curDim) {
    Point<Dim> pivotValue = list[pivotIndex];
    Point<Dim> temp = list[pivotIndex];
    list[pivotIndex] = list[right];
    list[right] = temp;
    int storeIndex = left;
    for (int i = left; i < right; i++) {
        if (smallerDimVal(list[i], pivotValue, curDim)) {
            Point<Dim> swapper = list[i];
            list[i] = list[storeIndex];
            list[storeIndex] = swapper;
            storeIndex++;
        }
    }
    Point<Dim> final_temp = list[right];
    list[right] = list[storeIndex];
    list[storeIndex] = temp;
    return storeIndex;
}

template<int Dim>
Point<Dim> KDTree<Dim>::select(vector<Point<Dim>> &list, int left, int right, int k, int curDim) {
    if (left == right) {
        return list[left];
    }
    int pivotIndex = (left + right) / 2;
    pivotIndex = partition(list, left, right, pivotIndex, curDim);
    if (k == pivotIndex) {
        return list[k];
    } else if (k < pivotIndex) {
        return select(list, left, pivotIndex - 1, k, curDim);
    } else {
        return select(list, pivotIndex + 1, right, k, curDim);
    }
}

template<int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::createNode(vector<Point<Dim>> &subList, int curDim) {
    if (subList.size() == 1) {
        KDTreeNode *newNode = new KDTreeNode(subList[0]);
        return newNode;
    }
    if (subList.size() == 2) {
        if(smallerDimVal(subList[0],subList[1],curDim)){
            KDTreeNode *newNode = new KDTreeNode(subList[0]);
            newNode->right = new KDTreeNode(subList[1]);
            return newNode;
        } else{
            KDTreeNode *newNode = new KDTreeNode(subList[1]);
            newNode->right = new KDTreeNode(subList[0]);
            return newNode;
        }
    }
    int mid = ((int) subList.size() - 1) / 2;
    KDTreeNode *newNode = new KDTreeNode(select(subList, 0, (int) subList.size() - 1, mid, curDim));
    auto itr = subList.begin();
    auto itr_mid = subList.begin() + mid;
    vector<Point<Dim>> leftPart;
    leftPart.assign(itr, itr_mid);
    vector<Point<Dim>> rightPart;
    rightPart.assign(itr_mid + 1, subList.end());
    curDim++;
    newNode->left = createNode(leftPart, curDim % Dim);
    newNode->right = createNode(rightPart, curDim % Dim);
    return newNode;
}

template<int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>> &newPoints) {
    /**
     * @todo Implement this function!
     */
    if (newPoints.empty()) {
        this->size = 0;
        this->root = nullptr;
    } else {
        int dim = 0;
        this->size = newPoints.size();
        vector<Point<Dim>> dynamicPoints = newPoints;
        int mid = ((int) dynamicPoints.size() - 1) / 2;
        root = new KDTreeNode(select(dynamicPoints, 0, (int) dynamicPoints.size() - 1, mid, dim));
        auto itr = dynamicPoints.begin();
        auto itr_mid = dynamicPoints.begin() + mid;
        vector<Point<Dim>> leftPart;
        leftPart.assign(itr, itr_mid);
        vector<Point<Dim>> rightPart;
        rightPart.assign(itr_mid + 1, dynamicPoints.end());
        dim++;
        if (!leftPart.empty()) {
            root->left = createNode(leftPart, dim % Dim);
        }
        if (!rightPart.empty()) {
            root->right = createNode(rightPart, dim % Dim);
        }
    }
}

template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copyHelper(KDTreeNode *subRoot) {
    KDTreeNode *newNode = new KDTreeNode();
    if (subRoot == NULL) {
        return NULL;
    } else {
        newNode->point = subRoot->point;
        newNode->left = copyHelper(subRoot->left);
        newNode->right = copyHelper(subRoot->right);
    }
    return newNode;
}


template<int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim> &other) {
    /**
     * @todo Implement this function!
     */
    this->size = other.size;
    this->root = copyHelper(other.root);
}

template<int Dim>
const KDTree<Dim> &KDTree<Dim>::operator=(const KDTree<Dim> &rhs) {
    /**
     * @todo Implement this function!
     */
    this->size = rhs.size;
    this->root = copyHelper(rhs.root);
    return *this;
}

template<int Dim>
KDTree<Dim>::~KDTree() {
    /**
     * @todo Implement this function!
     */
    del(this->root);
}

template<int Dim>
void KDTree<Dim>::del(KDTree<Dim>::KDTreeNode *node) {
    if (node != NULL) {
        del(node->left);
        del(node->right);
        delete node;
    }
}

template<int Dim>
Point<Dim> KDTree<Dim>::findBest(Point<Dim> target, KDTree<Dim>::KDTreeNode *curr, int curDim) const {
    Point <Dim> currentBest;
    if (curr->right == curr->left) {
        return curr->point;
    }
    if (smallerDimVal(target, curr->point, curDim)) {
        if (curr->left == nullptr) {
            currentBest = curr->point;
        } else {
            currentBest = findBest(target, curr->left, (curDim + 1) % Dim);
            if (shouldReplace(target, currentBest, curr->point)) {
                currentBest = curr->point;
            }
        }
        double dis = 0;
        for (int i = 0; i < Dim; i++) {
            dis += (double) pow(currentBest[i] - target[i], 2);
        }
        if(dis >= (double) pow(curr->point[curDim] - target[curDim],2)){
            Point<Dim> temp;
            if(curr->right != nullptr){
                temp = findBest(target, curr->right, (curDim + 1) % Dim);
                if (shouldReplace(target, currentBest, temp)) {
                    currentBest = temp;
                }
            }
        }
    } else {
        if (curr->right == nullptr) {
            currentBest = curr->point;
        } else {
            currentBest = findBest(target, curr->right, (curDim + 1) % Dim);
            if (shouldReplace(target, currentBest, curr->point)) {
                currentBest = curr->point;
            }
        }
        double dis = 0;
        for (int i = 0; i < Dim; i++) {
            dis += (double) pow(currentBest[i] - target[i], 2);
        }
        if(dis >= (double) pow(curr->point[curDim] - target[curDim],2)){
            Point<Dim> temp;
            if(curr->left != nullptr){
                temp = findBest(target, curr->left, (curDim + 1) % Dim);
                if (shouldReplace(target, currentBest, temp)) {
                    currentBest = temp;
                }
            }
        }
    };
    return currentBest;

}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> &query) const {
    /**
     * @todo Implement this function!
     */
    Point<Dim> currBest = findBest(query, root, 0);
    return currBest;
}

