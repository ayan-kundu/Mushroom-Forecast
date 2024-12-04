# -*- coding: utf-8 -*-
"""linearRegression.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1j-6txhRPnueWQuSnw58G5je0MFKwPRGT
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import linear_model
from sklearn.model_selection import train_test_split

from sklearn import linear_model
from sklearn.metrics import accuracy_score
from sklearn.metrics import roc_auc_score

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

# Model input features: 7am,8am,9am and 10am
size_list=[mm20,mm21,mm22,mm23,mm24,mm25]
train_i=7# 7am to 10 am
input_size_list=[mm20[ :train_i],mm21[ :train_i],mm22[ :train_i],mm23[ :train_i],mm24[ :train_i],mm25[ :train_i]]
# Model output is data in 6 and 9 hours: 7pm and 10pm
test_i=-1;j=12# 7pm
output_6h_size_list=[mm20[test_i],mm21[test_i],mm22[test_i],mm23[test_i],mm24[test_i],mm25[test_i]]
#output_9h_size_list=[mm20[j],mm21[j],mm22[j],mm23[j],mm24[j],mm25[j]]
print(f'Input: {input_size_list}\nOutput(7pm): {output_6h_size_list}') #\nOutput(7pm): {output_9h_size_list}

# Structuring data for feeding
#input_set=np.array(input_size_list)
output_set=np.array(output_3h_size_list) # define output set fro training
# Look at the output entity structure, visualise each batches with input size and out put
input_set=np.reshape(input_size_list,(6,7))
print(f'Data under consideration(i/p,o/p):\n{input_set} with size {input_set.shape}')
print(f'{output_set} with size {output_set.shape}')

#Splitting the data into train and test set
# Fitting the model
lin_reg=linear_model.LinearRegression( )
log_reg=linear_model.LogisticRegression(penalty='l2', fit_intercept=True, intercept_scaling=1, class_weight=None, random_state=None, solver='lbfgs', max_iter=100, multi_class='auto', verbose=0, warm_start=False, n_jobs=None)
print('Fitting the model...')
lin_reg.fit(input_set,output_set)
#log_reg.fit(input_set,output_set)

# making test data for evaluating the model
mm26=list(df.iloc[7,2:-1])# 7 for mm26
mm30=list(df.iloc[11,2:-1])# 11 for mm30
# test input
test_input_size_list=[mm26[ :train_i],mm30[ :train_i]]
print(f'Test input 26mm & 30mm{test_input_size_list}')
#test output
test_i=-1;# 7pm
test_6h_size_list=[mm26[test_i],mm30[test_i]]
#test_12_size_list=[mm26[j],mm30[j]]
print(f'Test output 26mm & 30mm @4pm {test_6h_size_list}')
#print(f'Test output 26mm & 30mm @7pm {test_12_size_list}')

print('...Ready to predict')
# Make predictions for test data
predictions=lin_reg.predict(test_input_size_list);predictions=[round(value,3) for value in predictions];
print(f'predictions: {predictions}')

print(f'\nTest vs Prediction comparison 26mm:: {test_6h_size_list[0]}-> {predictions[0]} &30mm ::{test_6h_size_list[1]}->{predictions[1]}')

s='@30mm'
train_label = np.linspace(0,train_i, num=7)
test_label = np.linspace(train_i+6,train_i+7, num=1)
forecast_label=test_label
# Display output graph
plt.figure(figsize =(10,5))
plt.xlabel('Hours');plt.ylabel('Growth(mm)')
plt.title(f'Forecast{s}')

i=1# 26mm
#Show training data
plt.plot(train_label,test_input_size_list[i],label='preforecast data'); #[0]--> 26mm
plt.plot(test_label,test_6h_size_list[i],'x',label='Test data in 6hrs')
plt.plot(forecast_label,predictions[i],'o',label='Forecast data in 6hrs') #predictions[0][0] [val,std]
#Arima_model.plot_predict(dynamic=False)
plt.legend()
plt.show()
#print(test_input_size_list[i],test_6h_size_list[i],predictions[i][0])