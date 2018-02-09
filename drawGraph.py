import sys

result=''
offset = 100
max_y = max_x = min_y = min_x = 0


class Point:

    def __init__(self,s):
        global max_y, max_x, min_y, min_x
        temp = s.split(",")

        self.x = int(temp[0])*10
        self.y = int(temp[1])*10
        if self.x < min_x:
            min_x = self.x
        if self.y < min_y:
            min_y = self.y
        if self.x > max_x:
            max_x = self.x
        if self.y > max_y:
            max_y = self.y

def addLine(i,s):
    global result
    result+= '\t'*i + s + '\n'

def _drawPoint(p,color):
    addLine(1,'<circle cx="%s" cy="%s" r="0.5" fill="%s" />' % (p.x,-1*p.y,color))

def drawPoint(p):
    _drawPoint(p,'blue')


def drawStart(p):
    _drawPoint(p,'orange')

def drawEnd(p):
    _drawPoint(p,'green')

def stringWithPoints(l):
    points=''
    for p in l:
        points+= str(p.x) + "," + str(-1*p.y) + " "

    points = points.strip()
    return points


def drawPolygon(l):
    points = stringWithPoints(l) 
    addLine(1,'<polygon stroke="black" stroke-width="0.1" fill="none"  points="%s"/>' % points)

def drawRoute(l):
    points = stringWithPoints(l)
    addLine(1,' <polyline stroke="red" stroke-width="0.2" fill="none" points="%s"/>' % points)

def printText(x,y,s):
    addLine(1,'<text x="%s" y="%s" font-family="Verdana" font-size="5">%s</text>'% (str(0),str(-1*(max_y+5)),s))


def main():
    lines = sys.stdin.readlines()
    title = lines[0][:-1]
    start = Point(lines[1])
    end   = Point(lines[2])
    polygons = []
    
    i = int(lines[3])    
    for line in lines[4:4+i]:
        subline = line.strip().split(" ")
        polygon = []
        for l in subline[1:]:
            polygon.append(Point(l))
        polygons.append(polygon)
    addLine(0,'<?xml version="1.0" encoding="UTF-8" ?>')
    viewbox = '%s %s %s %s' % (min_x-5, -1*(max_y+15), abs(min_x)+abs(max_x)+10, abs(min_y)+abs(max_y)+20)
    addLine(0,'<svg viewBox="%s" xmlns="http://www.w3.org/2000/svg" version="1.1">' % viewbox)
    drawStart(start)
    drawEnd(end)
    for polygon in polygons:
        drawPolygon(polygon)

    l =[Point(s) for s in lines[4+i].strip().split(" ")]
    drawRoute(l)

    # print(max_x,max_y,min_x,min_y)
    text = "%s, length: %s" % (title,lines[5+i])
    printText(0,0,text)
    # printText(max_x,max_y,lines[5+i])

    addLine(0,'</svg>')

    print(result)

if __name__ == "__main__":
    main()
