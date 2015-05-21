
import copy, types

def get_intersection(p11,p12,p21,p22):
    """
    Simple function to get a intersection of two lines defined by their endpoints
    """
    if not p11 or not p12 or not p21 or not p22:
        return []
    if p11==p12 or p21==p22:
        return []
    x1 = p11[0]; y1 = p11[1]
    x2 = p12[0]; y2 = p12[1]
    x3 = p21[0]; y3 = p21[1]
    x4 = p22[0]; y4 = p22[1]
    m1 = (y2-y1)/(x2-x1)
    m2 = (y4-y3)/(x4-x3)
    x = -1
    y = -1
    point = []
    if m1 != 0.0 and m2 != 0.0 and m1 != m2:
        x = ((y3-y1)+(m1*x1-m2*x3))/(m1-m2)
        y = ((x3-x1)+(y1/m1-y3/m2))/(1.0/m1-1.0/m2)
    else:
        if m1 == 0.0:
            if y4 >= y1 and y3 <= y1:
                y = y1
                x = (1/m2)*(y-y3) + x3
        elif m2 == 0.0:
            if y2 >= y3 and y1 <= y3:
                y = y3
                x = (1/m1)*(y-y1) + x1
        else: # same slope
            y = (x3-x1)*(y2-y1)/(x2-x1) + y1
            if y == y3:
                x = x3
            else:
                x = -1
    if x >= x1 and x <= x2 and x >= x3 and x <= x4 and y >= y1 and y <= y2 and y >= y3 and y <= y4:
        point = [x,y]
    else:
        point = [-1,-1]

    return point

def get_point_by_x(xdata,ydata,x):
    point = []
    for i in range(0,len(xdata)):
        if xdata[i] == x:
            point = [xdata[i],ydata[i]]
            break
        elif i < len(xdata) - 1:
            if xdata[i] < x and xdata[i+1] > x:
                point = [xdata[i],ydata[i]]
                break
    return point

def get_point_by_y(xdata,ydata,y):
    point = []
    for i in range(0,len(ydata)):
        if ydata[i] == y:
            point = [xdata[i],ydata[i]]
            break
        elif i < len(ydata) - 1:
            if ydata[i] < y and ydata[i+1] > y:
                point = [xdata[i],ydata[i]]
                break
    return point

def get_value_by_x(xdata,ydata,x):
    retval = 0
    point = get_point_by_x(xdata,ydata,x)
    if point:
        retval = point[0]
    return retval

def get_interp_by_x(xdata,ydata,x):
    retval = 0
    point = get_point_by_x(xdata,ydata,x)
    if point:
        point_ind = xdata.index(point[0])
        if point[0] == x:
            retval = point[1]
        else:
            retval = (x-xdata[point_ind])*(ydata[point_ind+1]-ydata[point_ind])/(xdata[point_ind+1]-xdata[point_ind]) + ydata[point_ind]
    else:
        retval = ydata[-1]
    return retval

def get_interp_by_y(xdata,ydata,y):
    retval = 0
    point = get_point_by_y(xdata,ydata,y)
    #print point
    if point:
        point_ind = ydata.index(point[1])
        while point_ind < len(ydata) - 1 and ydata[point_ind] == ydata[point_ind+1]:
            point_ind = point_ind + 1
        #print point_ind
        if point[1] == y:
            retval = point[0]
        else:
            if ydata[point_ind+1] != ydata[point_ind]:
                retval = (y-ydata[point_ind])*(xdata[point_ind+1]-xdata[point_ind])/(ydata[point_ind+1]-ydata[point_ind]) + xdata[point_ind]
            else:
                retval = xdata[point_ind]
    else:
        retval = xdata[-1]
    return retval

