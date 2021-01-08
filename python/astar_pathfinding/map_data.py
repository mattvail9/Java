import heapq
import map_data
import enum
from config import *

directions = ((0, 1), (1, 0), (0, -1), (-1, 0))

class PathType(enum.Enum):
    NORMAL = 0
    TOUCH = 1
    
def heuristic(a, b):
    return abs(a.x - b.x) + abs(a.y - b.y)
    
def retrace_path(node):
    path = []
    #rint('g', node.g_score)
    while not node is None:
        path.append((node.x, node.y))
        node = node.parent
    #rint('p',len(path))
    return path
    
def search(ax,ay,bx,by,path_type):
    data = map_data.data
    open_set = PriorityQueue()
    closed_set = {} #dictionary
    
    start = Node(ax, ay)
    goal = Node(bx, by)
    start.h_score = heuristic(start, goal)
    
    open_set.put(start)

    while open_set.empty() == False:

        current = open_set.get()

        if current == goal:
            if path_type == PathType.TOUCH:
                return retrace_path(current.parent)
            else:
                return retrace_path(current)
        
        closed_set[current] = current
        
        c = data[current.x][current.y]
        #print('c',c)
        #print(current.x, current.y,' --open set: ', len(open_set.elements))
        move_score = current.g_score + 1    #cost to node from start + neighbour edge cost
        
        #if c == LADDER or c == EMPTY:
        if current.y - 1 >= 0:
            ceil_tile = data[current.x][current.y - 1]
            if (ceil_tile == LADDER or ceil_tile == EMPTY) and c == LADDER:
                up_node = Node(current.x, current.y - 1)
                if not up_node in closed_set and not up_node in open_set.elements:
                    #print(current.x, current.y - 1, 'up added')
                    up_node.assign(heuristic(goal, up_node), move_score, current)
                    open_set.put(up_node)
               
        if current.y + 1 < MAP_HEIGHT:
            if data[current.x][current.y + 1] == LADDER:
                down_node = Node(current.x, current.y + 1)
                if not down_node in closed_set and not down_node in open_set.elements:
                    down_node.assign(heuristic(goal, down_node), move_score, current)
                    open_set.put(down_node)
        
        
        if current.x + 1 < MAP_WIDTH:
            right = data[current.x + 1][current.y]
            if is_solid(right) == False:
                floor_tile = map_data.get_data(current.x + 1, current.y + 1)
                if right == LADDER or is_solid(floor_tile) or floor_tile == LADDER:
                    right_node = Node(current.x + 1, current.y)
                    if not right_node in closed_set and not right_node in open_set.elements:
                        #print(current.x + 1, current.y,'right added')
                        right_node.assign(heuristic(goal, right_node), move_score, current)
                        open_set.put(right_node)
        
        if current.x - 1 >= 0:
            left = data[current.x - 1][current.y]
            if is_solid(left) == False:
                floor_tile = map_data.get_data(current.x - 1, current.y + 1)
                if left == LADDER or is_solid(floor_tile) or floor_tile == LADDER:
                    left_node = Node(current.x - 1, current.y)
                    if not left_node in closed_set and not left_node in open_set.elements:
                        #print(current.x - 1, current.y,'left added')
                        left_node.assign(heuristic(goal, left_node), move_score, current)
                        open_set.put(left_node)
                        
                    
    #print('get_path returning none','s:',ax,ay,'g:',bx,by, 'closed set len', len(closed_set))
    return None
                
def add_node(x, y, current, open_set):
    neighbor = Node(current.x, current.y + 1)
    move_score = current.g_score + 1    #cost to node from start + neighbour edge cost

    if not neighbor in open_set.elements or move_score < neighbor.g_score:
        neighbor.h_score = heuristic(goal, neighbor)
        neighbor.g_score = move_score
        neighbor.parent = current
        open_set.put(neighbor)
        
def get_path(start_x, start_y, goal_x, goal_y, path_type):
    print('s:',start_x,start_y, 'g:', goal_x, goal_y)

    data = map_data.data
    
    open_set = PriorityQueue()
    closed_set = {} #dictionary
    
    start = Node(start_x, start_y)
    goal = Node(goal_x, goal_y)
    
    start.h_score = heuristic(start, goal)
    
    open_set.put(start)
    
    test_breaker = 0
    while open_set.empty() == False:
    
        test_breaker = test_breaker + 1
        if test_breaker > 200:
            print('AStar breaker reached')
            break
            
        current = open_set.get()
        #rint(current.h_score)
        if current == goal:
            #rint('C:', current.x, current.y, 'G:', goal.x, goal.y)
            if path_type == PathType.TOUCH:
                return retrace_path(current.parent)
            else:
                return retrace_path(current)
        
        closed_set[current] = current
        
        ''''''
        #cur_tile = data[x][y]
        
        for d in directions:
            x = d[0] + current.x
            y = d[1] + current.y
  
            if x < 0 or x >= MAP_WIDTH or y < 0 or y >= MAP_HEIGHT:
                continue
            
            tileData = data[x][y]
            
            #if tileData < DIRT and data[x][y + 1] < LADDER:
                
           
            #print('current:',x, y,'t:',tileData)

            neighbor = Node(x,y)
            
            if neighbor in closed_set:
                continue
                
            move_score = current.g_score + 1    #cost to node from start + neighbour edge cost

            if not neighbor in open_set.elements or move_score < neighbor.g_score:
                #print('valid:', x, y)
                neighbor.h_score = heuristic(goal, neighbor)
                neighbor.g_score = move_score
                neighbor.parent = current
                open_set.put(neighbor)
                
            '''#print(tileData)
            if is_solid(tileData): #if soild, skip
                continue
            
            if is_solid(data[x][y + 1]) == False:
                continue
            
            if is_solid(data[x][y + 1]) == False: #if not solid, empty. check south tile for solid
                continue
                #bottom = y + 1               #check tile beneith for walkability
                #if bottom >= height or is_traversable(data[x][bottom] == False):
                    #continue
            #elif tileData < 4:
                #continue
            #if neighbour is greater than 4, LADDER, add node
            #if neighbour is open and has ground below, add node '''
            
            #tile edge move cost??
            
    
    print('get_path returning none, closed set len', len(closed_set))
    return None
  
class Node:
         
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.g_score = 0   #distance from start
        self.h_score = 0   #distance from end
        self.parent = None     #parent node
        
    def assign(self, g_score, h_score, parent):
        self.g_score = g_score
        self.h_score = h_score
        self.parent = parent
        
    def __eq__(self, other):
        return self.__hash__() == other.__hash__()
        
    def __ne__(self, other):
        return not self.__eq__(other)
        
    def __hash__(self):
        return hash((self.x, self.y))
        
    def __lt__(self, other):
        return self.f_score() < other.f_score()
        
    def f_score(self):
        return self.g_score + self.h_score
        
    def __str__(self):
        return 'g: {}, h: {}, f: {}'.format(self.g_score, self.h_score, self.f_score()) 


class PriorityQueue:
    def __init__(self):
        self.elements = []
        
    def empty(self):
        return len(self.elements) == 0
        
    def put(self, n):
        heapq.heappush(self.elements, (n.f_score(), n))
        
    def get(self):
        return heapq.heappop(self.elements)[1] #pulls node from (,)
