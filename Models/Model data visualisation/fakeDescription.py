# -*- coding: utf-8 -*-
"""Untitled0.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1HQ-bOsgpwXS1Rw8kWpavEvc3cXm-UPc6
"""

import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from scipy import stats as stat
import math


#import data 
file_path="C:/Users/Staff/Documents/Agaricus/Mushroom Forecast/Models/Model data visualisation/Fake data/Fake_data.xlsx"
df = pd.read_excel(file_path)
print(df[0:5])  # print first 5 rows of the dataframe

# import data scanned over growth on hourly basis 
sizes=['@20mm','@21mm','@22mm','@23mm','@24mm','@25mm']

# growth every hours over 12 hours of time
mm20=list(df.iloc[1,2:-1])
mm21=list(df.iloc[2,2:-1])
mm22=list(df.iloc[3,2:-1])
mm23=list(df.iloc[4,2:-1])
mm24=list(df.iloc[5,2:-1])
mm25=list(df.iloc[6,2:-1])

mm20_des=stat.describe(mm20)
mm21_des=stat.describe(mm21)
mm22_des=stat.describe(mm22)
mm23_des=stat.describe(mm23)
mm24_des=stat.describe(mm24)
mm25_des=stat.describe(mm25)

# descriptive statistics
print('<<<Description over Whole dataset 12hrs >>>',sizes)
print('\n1. Descriptive statistics: \n',mm20_des,'\n',mm21_des,'\n',
                            mm22_des,'\n',mm23_des,'\n',
                            mm24_des,'\n',mm25_des)
print('\n2. Standard Deviation: \n',math.sqrt(stat.describe(mm20).variance),
    '\n',math.sqrt(stat.describe(mm21).variance),'\n',
    math.sqrt(stat.describe(mm22).variance),'\n', math.sqrt(mm23_des.variance),'\n',
    math.sqrt(mm24_des.variance),'\n',math.sqrt(mm25_des.variance))

# Stationarity check 
from statsmodels.tsa.stattools import adfuller
def stationarity_test(dataset):
  test = adfuller(dataset, autolag = 'AIC')
  print("A. ADF : ",test[0])
  print("B. P-Value : ", test[1])
  print("C. Num Of Lags : ", test[2])
  print("D. Num Of Observations Used For ADF Regression:",test[3])
  print("E. Critical Values :")
  for key, val in test[4].items():
    print("\t",key, ": ", val)
  return test[1]
print(f'\n3. Stationarity Check:')
data1=mm20
data2=mm21
print(stationarity_test(data1));print(stationarity_test(data2))
print(['Data stationary' if stationarity_test(data1) <0.05 else 'Non stationary'])



