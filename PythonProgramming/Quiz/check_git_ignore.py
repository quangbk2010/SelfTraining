# -*- coding: utf-8 -*-
"""
Created on Thu Jul  6 18:52:36 2017

@author: quang
"""

pattern = ['.o\n', '.d\n', '.o\n', '.ko\n', '.obj\n', '.elf\n', '.ilk\n', '.map\n', '.exp\n', '.lib\n', '.a\n', '.la\n', '.lo\n', '.so\n', 'dylib\n', 'cmd\n', '.old\n', 'fixdep\n', '.status\n', '.config\n', '.order\n', '/lib\n', '.in\n', '.5\n', '.8\n', 'test\n', '.Plo\n', '.dist\n']
f1 = open ('gitstatus.txt', 'r')
f2 = open ('remainstatus.txt', 'w')

for line in f1:
    end2 = line[-3:]
    end3 = line[-4:]
    end4 = line[-5:]
    end5 = line[-6:]
    end6 = line[-7:]
    end7 = line[-8:]
    #print (end2)
    if end4 == '/lib\n':
        print (line)
    #if (end2 in pattern) or (end3 in pattern) or (end4  in pattern) or (end5 in pattern) or (end6 in pattern) or (end7 in pattern):    
        #print (line)
        #break
    if (end2 not in pattern) and (end3 not in pattern) and (end4 not in pattern) and (end5 not in pattern) and (end6 not in pattern) and (end7 not in pattern):
        f2.write (line)
f1.close()
f2.close()