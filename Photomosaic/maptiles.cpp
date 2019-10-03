/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "cs225/HSLAPixel.h"
#include "maptiles.h"
#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

    MosaicCanvas *theCanvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    vector<Point<3>> newPoints;
    map<Point<3>, int> medium;
    for (unsigned i = 0; i < theTiles.size(); i++) {
        newPoints.push_back(convertToXYZ(theTiles[i].getAverageColor()));
        medium.insert(pair<Point<3>, int>(convertToXYZ(theTiles[i].getAverageColor()), i));
    }
    KDTree<3> avgColors = KDTree<3>(newPoints);
    for (int i = 0; i < theCanvas->getRows(); i++) {
        for (int j = 0; j < theCanvas->getColumns(); j++) {
            Point<3> target = convertToXYZ(theSource.getRegionColor(i, j));
            Point<3> neighbor = avgColors.findNearestNeighbor(target);
            theCanvas->setTile(i, j, &theTiles[medium[neighbor]]);
        }
    }
    return theCanvas;
}

