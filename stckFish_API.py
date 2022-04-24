from operator import truediv
from tkinter import Y
from stockfish import Stockfish
import serial
import time

#global variables
pos_x=1
pos_y=1

dead_x= 9
dead_y=1

x_max=99
y_max=99

userMov_flag=1 #if 1 then user has to move it
castle_flag=0
st=0

# def quit_fun(dict_user, dict_comp):
#     key_list= list(dict_user_global.keys())
#     value_list= list(dict_user_global.values())
#     for x,y in dict_user:
#         if(key_list):

def arduino_set():
    global arduino 
    arduino = serial.Serial(port='/dev/ttyS6', baudrate=9600, timeout=.1)

def arduino_wait(arduino):
    data = arduino.readline()
    data= data.decode()
    while(data!="69\r\n"):
        data = arduino.readline()
        data= data.decode()
    print("Worked")

def arduino_move(e1,x,y,e2):
    arduino.write(bytes(str(e1), 'utf-8'))
    time.sleep(.1)
    arduino_wait(arduino)
    arduino.write(bytes(str(x), 'utf-8'))
    time.sleep(1)
    arduino_wait(arduino)
    arduino.write(bytes(str(y), 'utf-8'))
    time.sleep(1)
    arduino_wait(arduino)
    arduino.write(bytes(str(e2), 'utf-8'))
    time.sleep(1)
    arduino_wait(arduino)



def mapping_remove(des):
    global x_max, y_max, pos_x, pos_y, dead_x, dead_y
    
    x_des= des[0]
    y_des= des[1]
   
    dist_x= ord(x_des)-96-pos_x
    dist_y= int(y_des)- pos_y
    #move dist_x*square_length and dist_y*square_length with end effector down
    #wait till this is done

    pos_x= dist_x+pos_x
    pos_y= dist_y+pos_y

    if(pos_x>x_max):
        print("OUTSIDE BOUNDS\n")
    else:
        print(pos_x)
        print(pos_y)
        arduino_move(0,pos_x, pos_y,1)

    #dist_x= dead_x-pos_x
    #dist_y= dead_y- pos_y

    dist_x= 9-pos_x
    dist_y=0
    

    #move dist_x*square_length and dist_y*square_length with end effector up
    #wait till this is done
    pos_x= dist_x+pos_x
    pos_y= dist_y+pos_y

    if(pos_x>x_max):
        print("OUTSIDE BOUNDS\n")
    else:
        print(pos_x)
        print(pos_y)
        arduino_move(1,pos_x, pos_y,0)        
        dead_y= dead_y+1


def mapping_move(src, des):
    global square_length, pos_x, pos_y, dead_x, dead_y
    x_src= src[0]
    y_src= src[1]
    dist_x= ord(x_src)-96-pos_x
    dist_y= int(y_src)- pos_y

    #move dist_x*square_length and dist_y*square_length with end effector down
    #wait till this is done
    pos_x= dist_x+pos_x
    pos_y= dist_y+pos_y

    if(pos_x>x_max):
        print("OUTSIDE BOUNDS\n")
    else:
        print(pos_x)
        print(pos_y)
        arduino_move(0,pos_x, pos_y,1)

    x_des= des[0]
    y_des= des[1]
    dist_x= ord(x_des)-96-pos_x
    dist_y= int(y_des)- pos_y
    #move dist_x*square_length and dist_y*square_length with end effector up
    #wait till this is done
    
    pos_x= dist_x+pos_x
    pos_y= dist_y+pos_y

    if(pos_x>x_max) or (pos_y>y_max):
        print("OUTSIDE BOUNDS\n")
    else:
        print(pos_x)
        print(pos_y)
        arduino_move(1,pos_x, pos_y,0)

    

    #move end effector down    


def valid_move(stockfish, dict,dict_des, mov,col):
    valid_flag= stockfish.is_move_correct(mov)
    global castle_flag
    if valid_flag:
        source= mov[0:2]
        des= mov[2:4]

        if(len(mov)>4):
            src2= mov[5:7]
            des2= mov[7:9]
            castle_flag=1

        

        key_dest= dict_des.get(des)
        if(key_dest!=None):
            dict_des.pop(des)
            if(userMov_flag==0):
                mapping_remove(des)
            else:
                if(col==0):
                    mapping_remove(des)
        
        key=dict.get(source)
        if(key == "k" or key=="K"):
            if(abs(ord(des[0])-ord(source[0]))>1):
                castle_flag=1
                if(des[0]=="G" or des[0]=="g"):
                    src2= chr(ord(des[0])+1)+ des[1]
                    des2= chr(ord(des[0])-1)+ des[1]
                elif(des[0]=="C" or des[0]=='c'):
                    src2= src2= chr(ord(des[0])-2)+ des[1]
                    des2= chr(ord(des[0])+1)+ des[1]
        dict.pop(source)
        dict.update({des:key})

        if(castle_flag==1):
           key=dict.get(src2)
           dict.pop(src2)
           dict.update({des2:key}) 


        if(userMov_flag==0):
            if(castle_flag==0):
                mapping_move(source, des)
            else:
                mapping_move(source,des)
                mapping_move(src2,des2)

        else:
            if(col==0):
                mapping_move(source, des)
                if(castle_flag==1):
                   mapping_move(src2,des2) 

        castle_flag=0
        return True

    else:
        return False



# need to check for queen replacement


path_dest = r"/mnt/c/Personal/Spring_2022/ME_360/Assignments/Autonomous_Chess/stockfish14.exe"

stockfish = Stockfish(path= path_dest)
arduino_set()



skill= input("Please choose the difficulty (1-20)? 1 being the easiest and 20 being the most difficult\n")

stockfish.set_skill_level(int(skill))

global dict_user_global
global dict_comp_global
flag = True

dict_white= {"a2":"P1","b2":"P2","c2":"P3","d2":"P4","e2":"P5","f2":"P6","g2":"P7","h2":"P8"
, "a1":"R1","b1":"N1","c1":"B1","d1":"Q","e1":"K","f1":"B2","g1":"N2","h1":"R2"}

dict_black= {"a7":"p1","b7":"p2","c7":"p3","d7":"p4","e7":"p5","f7":"p6","g7":"p7","h7":"p8"
, "a8":"r1","b8":"n1","c8":"b1","d8":"q","e8":"k","f8":"b2","g8":"n2","h8":"r2"}


decision= input("White (W) or Black(B):")
dec_flag =True

while(dec_flag):
    if (decision == 'w'or decision =="W" or decision == "white" or decision == "White"):
        dict_user= dict_white
        dict_comp= dict_black
        dict_user_global= dict_white
        dict_comp_global = dict_black
        dec_flag= False

    elif (decision == 'b'or decision =="B" or decision == "black" or decision == "Black"):
        dict_comp= dict_white
        dict_user = dict_black
        dict_user_global= dict_black
        dict_comp_global = dict_white
        comp= stockfish.get_best_move()
        valid_move(stockfish, dict_comp,dict_user, comp,0)
        list_comp= [comp]
        stockfish.make_moves_from_current_position(list_comp)

        dec_flag= False

    elif( decision == "quit"):
        quit()

    else:
        print("Invalid Input")
        decision= input("White (W) or Black(B):")
        dec_flag= True


print(stockfish.get_board_visual())

while(flag):
    if(st==1):
        mov= stockfish.get_best_move()
    else: 
        mov= input("Enter next move:")
    if(mov=="quit"):
        arduino_move(0,1,1,0)
        quit()
    else:
        while not(valid_move(stockfish, dict_user,dict_comp, mov,1)):
            print("Not a valid move")
            mov= input("Enter new move:")            
            if(mov=="quit"):
                arduino_move(0,1,1,0)
                quit()
        list_mov= [mov]
        stockfish.make_moves_from_current_position(list_mov)


        str_mate= stockfish.get_best_move()
        if(str_mate ==None):
            print("Congrats! You won.")
            arduino_move(0,1,1,0)
            var= stockfish.get_board_visual()
            print(var)
            quit()

        comp= stockfish.get_best_move()
        list_comp= [comp]
        valid_move(stockfish, dict_comp,dict_user, comp,0)
        stockfish.make_moves_from_current_position(list_comp)

        var= stockfish.get_board_visual()
        print(var)
        
        str_mate= stockfish.get_best_move()
        if(str_mate ==None):
            print("Good Try, but you lost!")
            arduino_move(0,1,1,0)
            quit()

        


