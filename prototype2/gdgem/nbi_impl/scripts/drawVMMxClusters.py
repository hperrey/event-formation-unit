#!/usr/bin/env python

import h5py as h5
import numpy as np
import matplotlib.pyplot as plt
import argparse

def findEaliestTime(obj):

    minTimeX = min(obj[0][0])
    minTimeY = min(obj[1][0])

    return min([minTimeX, minTimeY])

def adjustTime(time, minTime):

    time[:] = [x - minTime for x in time]

def adjustClusterTime(obj):

    minTime = findEaliestTime(obj)

    adjustTime(obj[0][0], minTime)
    adjustTime(obj[1][0], minTime)

def drawPlane(cluster, ax, plane, zoomed=False):

    if plane:
        title = "Y-plane"
    else:
        title = "X-plane"

    if zoomed:
        title += " (Zoomed)"

    minS = min(cluster[plane][1])
    maxS = max(cluster[plane][1])
    minT = min(cluster[plane][0])
    maxT = max(cluster[plane][0])

    ax.set_title(title)
    ax.set_xlabel('Strip #')
    ax.set_ylabel('Time-bin')
    ax.grid(True)
    if zoomed:
        ax.set_xlim([minS-5, maxS+5])
        ax.set_ylim([minT-1, maxT+1])
    else:
        ax.set_xlim([0,512])
        ax.set_ylim([0,500])

    ax.plot(cluster[plane][1], cluster[plane][0], "ko", label="Original", markersize=5, mew=3)

def draw(cluster):

    ax[0][0].clear()
    ax[0][1].clear()
    ax[1][0].clear()
    ax[1][1].clear()

    adjustClusterTime(cluster)

    drawPlane(cluster, ax[0][0], 0)
    drawPlane(cluster, ax[0][1], 1)
    drawPlane(cluster, ax[1][0], 0, True)
    drawPlane(cluster, ax[1][1], 1, True)

    fig.show()

def readDataLine(file):

    line  = file.readline()
    words = line.split()

    return map(int, words)

def readPlane(file):

    line  = file.readline()
    words = line.split()

    if words[0] != "x-points:" and words[0] != "y-points:":
        print "Did not find plane header! Found: " + words[0]
        return

    plane = []

    plane.append(readDataLine(file))
    plane.append(readDataLine(file))
    plane.append(readDataLine(file))

    return plane

def readCluster(file):

    cluster = []

    cluster.append(readPlane(file))
    cluster.append(readPlane(file))

    return cluster

def readNextEvent(file):

    line  = file.readline()
    words = line.split()

    if words[0] != "******":
        print "Did not find event-header!"
        return

    cluster = readCluster(file)

    return cluster

### Main program

ifilename = "NMX_PairedClusters.txt"

parser = argparse.ArgumentParser()

parser.add_argument("-i", "--ifile", type=str, help="Input file")

args = parser.parse_args()

if args.ifile:
    ifilename = args.ifile

file = open(ifilename, "r")

bc = 32
maxb = 30
multiplier = 60

colors = ['b', 'r', 'c', 'm', 'y', 'g', 'w']

fig, ax = plt.subplots(2, 2, figsize=(10,10), facecolor='White')
plt.subplots_adjust(left=0.07, right=0.94, top=0.93, bottom=0.07,
                    wspace=0.22, hspace=0.25)

ievent = 0

while True:

    fig.suptitle('Event # ' + str(ievent), fontsize=20)

    cluster = readNextEvent(file)

    if cluster == None:
        break;

    draw(cluster)

    x = raw_input("Hit ENTER for next event, 's' to save")

    if x == 's' or x == 'S':
        name = ifilename.replace(".txt", '') + str(ievent).zfill(3) + ".pdf"
        fig.savefig(name)
        print "Wrote image to : " + name

    if x == 'q' or x == 'Q':
        quit()

    del cluster[:]

    ievent += 1
