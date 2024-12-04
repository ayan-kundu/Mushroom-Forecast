import numpy as np
import pandas as pd
from scipy import stats as stat
import math, statistics
import matplotlib.pyplot as plt

#import data
file_path="/content/Fake_data.xlsx"
df = pd.read_excel(file_path)
#print(df[0:5])  # print first 5 rows of the dataframe

mm20=list(df.iloc[1,2:-1])
mm21=list(df.iloc[2,2:-1])
mm22=list(df.iloc[3,2:-1])
mm23=list(df.iloc[4,2:-1])
mm24=list(df.iloc[5,2:-1])
mm25=list(df.iloc[6,2:-1])

size_list=[mm20,mm21,mm22,mm23,mm24,mm25]

mm20_des=stat.describe(mm20)
mm21_des=stat.describe(mm21)
mm22_des=stat.describe(mm22)
mm23_des=stat.describe(mm23)
mm24_des=stat.describe(mm24)
mm25_des=stat.describe(mm25)

mean_mm20=mm20_des.mean
mean_mm21=mm21_des.mean
mean_mm22=mm22_des.mean
mean_mm23=mm24_des.mean
mean_mm24=mm24_des.mean
mean_mm25=mm25_des.mean

# Checking Statistical values
print('*** Data Captured Every 1 Hour*** ')
print('Statistical description of data over each available size ')
for i in size_list:
  print(f'{stat.describe(i)} std: {math.sqrt(stat.describe(i).variance)}') # INFERENCE

hh=12 # Total time the growth is under consideration
print(f'\nGrowth over {hh} hours for each sizes from 20 to 25mm-')
# Make a function to give list of growth and Growth Rate (GR)
def growth_check(s_list,hh):
  for i in range (len(s_list)):
    growth=round(s_list[i][-1]-s_list[i][0],4) # calculation for *growth = (Final growth- Initial growth)
    growth_list.append(growth)
    growth_rate=growth/hh # calculation for *growth rate =(Total growth/Time of growth)
    gr_list.append(round(growth_rate,4))

#Checking growth and GR over data captured every 1 hour
growth_list=[];gr_list=[]
growth_check(size_list,12)
print('-----For each sizes from 20 to 25mm-----' )# INFERENCE
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}')

interval=3
print(f'\n\n *** Data Captured Every {interval} Hours*** ')

# growth every 3/6 hours

a=0;itr=5 # itr=5 every 3 hours # itr=3 every 6 hours
for i in range (itr):
    mm20[i]=mm20[a];mm21[i]=mm21[a];mm22[i]=mm22[a];mm23[i]=mm23[a];mm24[i]=mm24[a];mm25[i]=mm25[a]
    a+=3
mm20=mm20[:itr];mm21=mm21[:itr];mm22=mm22[:itr];mm23=mm23[:itr];mm24=mm24[:itr];mm25[:itr]
# Update size_list
size_list=[mm20,mm21,mm22,mm23,mm24,mm25]
# Checking Statistical values
print('Statistical description of data over each available size from 20 to 25mm')
for i in size_list:
  print(f'{stat.describe(i)} std: {math.sqrt(stat.describe(i).variance)}')


#Checking growth and GR over data captured every 3 hours
growth_list=[];gr_list=[]
growth_check(size_list,12)
print(f'\nGrowth over {hh} hours for each sizes from 20 to 25mm-')
print('-----For each sizes from 20 to 25mm-----' )
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}')

#print(' Growth and GR does not change (much?) even if there is interval in data fetch') # INFERENCE


# TEST OVER 1ST HALF OVER 2ND HALF #####
# refresh data loader
mm20=list(df.iloc[1,2:-1])
mm21=list(df.iloc[2,2:-1])
mm22=list(df.iloc[3,2:-1])
mm23=list(df.iloc[4,2:-1])
mm24=list(df.iloc[5,2:-1])
mm25=list(df.iloc[6,2:-1])

l=len(mm20)#13
size_list=[mm20,mm21,mm22,mm23,mm24,mm25]
size_list_1=[];size_list_2=[]
hh=interval=6
print(f'\n\n *** Data divided into Every {interval} Hours*** ')
for i in range (len(size_list)):
  mm_x_1=size_list[i][0:7];mm_x_2=size_list[i][6:];#print(mm_x_1,mm_x_2)
  print(f'Variance (difference) {stat.describe(mm_x_1).variance} {stat.describe(mm_x_2).variance} Skewness (difference) {stat.describe(mm_x_1).skewness} {stat.describe(mm_x_2).skewness}')
  size_list_1.append(mm_x_1);size_list_2.append(mm_x_2)


#print('Data divided into 2 sets\n',size_list_1,'\n',size_list_2)
print(f'\n---Growth over {hh} hours for each sizes from 20 to 25mm(in list)--- 7 am to 1 pm')
growth_list=[];gr_list=[]
growth_check(size_list_1,6)
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}')
print(f'---Growth over {hh} hours for each sizes from 20 to 25mm(in list)--- 1 pm to 7 pm')
growth_list=[];gr_list=[]
growth_check(size_list_2,6)
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}\n')


hh=interval=3
print(f'\n\n *** Data divided into Every {interval} Hours*** ')
#print('Data divided into 4 sets\n',size_list_1,'\n',size_list_2,size_list_3,'\n',size_list_4)

size_list_1=[];size_list_2=[];size_list_3=[];size_list_4=[]
for i in range (len(size_list)):
  mm_x_1=size_list[i][0:4];mm_x_2=size_list[i][3:7];mm_x_3=size_list[i][6:10];mm_x_4=size_list[i][9:]
  #print(mm_x_1,mm_x_2,mm_x_3,mm_x_4)
  print(f'Variance (difference) {stat.describe(mm_x_2).variance} {stat.describe(mm_x_3).variance} Skewness (difference) {stat.describe(mm_x_2).skewness} {stat.describe(mm_x_3).skewness}')
  size_list_1.append(mm_x_1);size_list_2.append(mm_x_2);size_list_3.append(mm_x_3);size_list_4.append(mm_x_4)

print(f'\n---Growth over {hh} hours for each sizes from 20 to 25mm(in list)--- 7 am to 10 am')
growth_list=[];gr_list=[]
growth_check(size_list_1,3)
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}')
GR_7am=gr_list

print(f'---Growth over {hh} hours for each sizes from 20 to 25mm(in list)--- 10 am to 1 pm')
growth_list=[];gr_list=[]
growth_check(size_list_2,3)
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}')
GR_10am=gr_list

print(f'---Growth over {hh} hours for each sizes from 20 to 25mm(in list)--- 1 pm to 4 pm')
growth_list=[];gr_list=[]
growth_check(size_list_3,3)
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}')
GR_1pm=gr_list

print(f'---Growth over {hh} hours for each sizes from 20 to 25mm(in list)--- 4 pm to 7 pm')
growth_list=[];gr_list=[]
growth_check(size_list_4,3)
print(f'Growth: {growth_list}')
print(f'Growth rate: {gr_list}\n')
GR_4pm=gr_list

# Graph
sizes=['@20mm','@21mm','@22mm','@23mm','@24mm','@25mm']
GR_20mm=[];GR_21mm=[];GR_22mm=[];GR_23mm=[];GR_24mm=[];GR_25mm=[]
SizeGR_list=[GR_20mm,GR_21mm,GR_22mm,GR_23mm,GR_24mm,GR_25mm]
plt.title("GR measured Every 3 Hours over 4 time windows")
plt.xlabel(" Time periods")
plt.ylabel("Growth rate(mm/h)")
for i in range(len(SizeGR_list)):
  SizeGR_list[i]=[GR_7am[i],GR_10am[i],GR_1pm[i],GR_4pm[i]]
  plt.plot(SizeGR_list[i],'--o')
plt.legend(sizes)
print(f'Growth rate for each size: {SizeGR_list}')

print('\n---To get generalaised growth rate for FC analysis---')
gen_GR_list=[]
#[[0.9333, 1.0, 1.0, 1.0667], [1.0, 1.1, 0.9, 1.0], [0.9333, 1.0667, 1.2, 0.8667], [1.0733, 1.15, 1.15, 0.8267], [1.0333, 1.0333, 0.9333, 1.0]]
for i in SizeGR_list:
  gen_GR_list.append(round(statistics.mean(i),3))
print(gen_GR_list)