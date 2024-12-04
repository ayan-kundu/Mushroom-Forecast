import numpy as np
import pandas as pd 
import matplotlib.pyplot as plt 

from sklearn.model_selection import train_test_split

from sklearn.metrics import accuracy_score
from sklearn.metrics import roc_auc_score

np.random.seed(1)
weather_data_set="C:/Users/Staff/Downloads/weather.csv"
#df=pd.read_csv(weather_data_set,sep=';')
df=pd.read_csv(weather_data_set)
df_array=np.asarray(df)
print('<<< Data frame >>> \n',df.head(5))

# Input features
rain_fall=df.iloc[0:200,2]
sun_shine=df.iloc[0:200,4]
wind_gust=df.iloc[0:200,6]
wind_speed_9am=df.iloc[0:200,9];wind_speed_9am.fillna(round(wind_speed_9am.mean(),1),inplace=True)
wind_speed_3pm=df.iloc[0:200,10]
# Output entity
rain_today=df.iloc[0:200,-3];rain_today.replace(('Yes','No'),(1,0),inplace=True)
#print(rain_fall,sun_shine,rain_today)

# Structuring data for feeding
x=np.array([rain_fall,sun_shine,wind_gust,wind_speed_9am,wind_speed_3pm])
# Look at the output entity structure, visualise each batches with input size and out put
x=np.reshape(x,(200,5))
y=np.array(rain_today)
print('Input Shape ',x.shape,' Output shape ',y.shape)

#Splitting the data into train and test set
split=10 # % is the  test set
x_train,x_test,y_train,y_test= train_test_split(x,y,test_size=split/100,random_state=42)

# Fitting the model
print('Fitting the model...')
# XGBoost model ###                #####
from xgboost import XGBClassifier
XGBoost=XGBClassifier()
# Fitting the model
XGBoost.fit (x_train,y_train)
# Make predictions for test data
predictions=XGBoost.predict(x_test);predictions=[round(value) for value in predictions];
# Performance Score
score=accuracy_score(y_test,predictions)
roc_value=roc_auc_score(y_test,predictions)
# Display model performance
print(f'XGBoost score: {score}% || ROC: {roc_value}')
print(predictions)

# SVR model ###                #####
from sklearn.svm import SVR
svr=SVR(C=1.0, epsilon=0.2)
svr.fit(x_train,y_train)
# Make predictions for test data
predictions=svr.predict(x_test);predictions=[round(val) for val in predictions]
# Performance Score
score=accuracy_score(y_test,predictions)
roc_value=roc_auc_score(y_test,predictions)
# Display model performance
print(f'SVR score: {score}% || ROC: {roc_value}')
print(predictions)

