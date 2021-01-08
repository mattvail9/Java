import pygame
from pygame.locals import *
import random
#from Unit import Unit
from sprite_atlas_functions import SpriteAtlas
from unit_manager import Unit
import map_data
import math
from config import *
import object_manager
import items
import gui


#Colors
BLACK = ( 0, 0, 125 )
BGCOLOR = ( 125, 125, 255 )

#Sprites
#TERRAIN = pygame.image.load('terrain.png')
#UNIT_IMAGE = pygame.image.load('unit.png')

class Camera:
    def __init__(self, x, y):
        self.x = x
        self.y = y
#---GUI------------------------------------------------------------------------------------------------------------        
def button_action(command):
    ACTIVE_COMMAND = command
    
#---MAIN GAME LOOP-------------------------------------------------------------------------------------------------------------------MAIN GAME LOOP---
def main_game_loop():
    '''Main game loop'''
    global DISPLAYSURF, FPSCLOCK, DECAL, decal_textures
  
    pygame.init()   #must call init before any pygame functions are used
    
    DISPLAYSURF = pygame.display.set_mode( (SCREENWIDTH, SCREENHEIGHT) )    #creates game window
    
    FPSCLOCK = pygame.time.Clock() #Use with .tick()
    
    quit_game = False

    map_data.init()
    DECAL = {}
    decal_textures = SpriteAtlas('decal.png', 32, 2, 2)
    all_sprites_list = pygame.sprite.Group() #Initializes Group
    
    #OBJECT_MANAGER = {}
    
    '''TESTING'''
    for i in range(5,12):
        map_data.data[8][i] = LADDER
        
    for j in range(7,10):
        map_data.data[12][j] = LADDER
        
    for x in range(9, 12):
        for y in range(7,10):
            map_data.data[x][y] = GRASSL
    
    #for i in range(25):
    unit = Unit(5, 9) #Base class in unit_manager
    all_sprites_list.add(unit)
    #/ [random.choice((-5, 5)), random.choice((-5, 5))])
    
       
       
    #fps vars
    fps_avg = 0.0
    fps_avg_cntr = 0
    fps_sum = 0.0
    fps_cntr = 0
    fps_text = pygame.font.SysFont('comicsansms.tff', 32)
    fps_low = 250
    fps_high = 0
    
    
    camera = Camera( SCREENWIDTH / 2, SCREENHEIGHT / 2 )
    #textRect = fps_text.render(str(30)).get_rect()
    '''path = pathfinding.get_path(map_data, MAP_WIDTH, MAP_HEIGHT, pathfinding.Node(0,0), pathfinding.Node(13,10))
    for pos in path:
        rint('{}, {}'.format(pos[0], pos[1]))'''
    #for t in range(2):
    map_data.data[15][9] = TREE
    
    #set stockpile position and add 5 wood   

    items.init((1,9))
    for i in range(2):
        items.add(items.WOOD)
        
    #test_surface = pygame.Surface((50,50),10, 10)
    #test_surface.set_colorkey
    gui.init()
    '''ENDTESTING'''
    
    #mouse control vars
    down_pos = None
    current_pos = None
    is_down = False
    
    paused = False
    #all_sprites_list = pygame.sprite.Group()
    #for run in range(25):
    while not quit_game:
#---EVENTS-------------------------------------------------------------------------------------------------------------------EVENTS---
        if is_down:
            cur_x, cur_y = pygame.mouse.get_pos()
        
        event_list = pygame.event.get()
           
        for event in event_list:
            if event.type == pygame.QUIT:
                quit_game = True
                pygame.quit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                is_down = True
                #print('Mouse Down', is_down)
                down_pos = pygame.mouse.get_pos()
            elif event.type == pygame.MOUSEBUTTONUP:
                is_down = False
                #print('Mouse Down', is_down)
                #x, y = pygame.mouse.get_pos()
                
                if gui.on_gui(cur_x,cur_y) == False:
                    x, y = screen_to_grid(cur_x, cur_y)
                    #add_decal(x, y, 0)
                    #print(x, y)
                    #do stuff at modified points
                #map_data.set_data(x//TILE_SIZE,y//TILE_SIZE, EMPTY)
                    #print(x,y, map_data.get_data(x, y) )
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_a:
                    camera.x = camera.x - 1
                elif event.key == pygame.K_d:
                    camera.x = camera.x + 1
                elif event.key == pygame.K_p:
                    paused = not paused
                elif event.key == pygame.K_s:
                    items.add(items.WOOD)
                elif event.key == pygame.K_x:
                    for item in items.item_storage:
                        if items.item_storage[item] > 0:
                            print(items.item_database[item], items.item_storage[item])
        if paused:
            continue
            
        
            
        all_sprites_list.update()
 
#---DRAW GAME-------------------------------------------------------------------------------------------------------------------DRAW GAME--- 
        DISPLAYSURF.fill(BGCOLOR)

        #Display FPS
        fps_cntr += 1
        fps = FPSCLOCK.get_fps()
        fps_sum = fps_sum + fps
        
        if fps < fps_low:
            fps_low = fps
        if fps > fps_high:
            fps_high = fps
            
        if fps_cntr >= 60:

            fps_avg_cntr += 1
            
            if fps_avg_cntr > 3:

                fps_avg = fps_sum / (fps_avg_cntr * fps_cntr)
                fps_sum = 0
                
                fps_avg_cntr = 0
                fps_low = 250
                fps_high = 0
            fps_cntr = 0    
#---Draw Game-----------------------------------------------------------------------         
        draw_map()

        DISPLAYSURF.blit(fps_text.render('{0:.2f}   {1:2.2f}   L:{2:.2f}   H:{3:.2f}'.format( fps, fps_avg, fps_low, fps_high ), 0, (0,0,0)), [0, 0])
        
        draw_decal()

        all_sprites_list.draw(DISPLAYSURF)
        
        
        gui.update(DISPLAYSURF)
        
        pygame.display.flip()
        
        FPSCLOCK.tick(FPS)      #Delays until FPS == 1sec, If code finishes before FPS timer
        
        #pygame.draw.rect(DISPLAYSURF, (45, 200, 90), (10, 60, 32, 32), 5)


def draw_map():
    global DISPLAYSURF
    for x in range(25):
        for y in range(25):
            if map_data.data[x][y] == -1:
                continue
            DISPLAYSURF.blit(map_data.sprite_atlas.get_image(map_data.data[x][y]), (x * TILE_SIZE, y * TILE_SIZE))

def add_decal(x, y, index):
    pos = (x, y)
    if not pos in DECAL:
        DECAL[pos] = index
        return True
    return False

def draw_decal():
    global DISPLAYSURF
    for k in DECAL:
        DISPLAYSURF.blit(decal_textures.get_image(DECAL[k]), (k[0] * TILE_SIZE, k[1] * TILE_SIZE))
    
main_game_loop()
