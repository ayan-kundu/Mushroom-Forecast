import numpy as np
import pandas as pd 
import matplotlib.pyplot as plt 

from sklearn.model_selection import train_test_split
from sklearn.gaussian_process import GaussianProcessRegressor
from sklearn.gaussian_process.kernels import RBF, ConstantKernel,ExpSineSquared,RationalQuadratic, WhiteKernel,  DotProduct

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

# Augmented Dickey-Fuller test # it helps see p-value for stationarity <0.05
# tsa:time series analysis
from statsmodels.tsa.stattools import adfuller
diagnosis= adfuller(x[0])
print(f'ADF STAT: {diagnosis[0]}  || P-value {diagnosis[1]}')
print('Critical Values:')
for key, value in diagnosis[4].items():
  print('\t%s: %.3f' % (key, value))

# Fetching p,d,q value : best model order 
from pmdarima import auto_arima
stepwise_fit = auto_arima(df['Rainfall'], trace=True,suppress_warnings=True) #not subscriptable # tries find out order with minimum AIC score
print('\n',stepwise_fit) # order=(0,1,1)
#stepwise_fit = auto_arima(y_train, trace=True,suppress_warnings=True)
#print('\n',stepwise_fit) # order=(0,0,0)

# Forecast outbound data using the model 
from statsmodels.tsa.arima.model import ARIMA # ARIMA with no seasonal variation
from statsmodels.tsa.statespace.sarimax import SARIMAX # SARIMAX with seasonal change/factors
Arima=ARIMA(x_train[0],order=(0,1,1)) # With this data the best order seems:(0,1,1) # ARIMA(x_train,order) <<< X
Arima_model=Arima.fit()
Arima_model.summary()

# ARIMA Prediction
predictions = Arima_model.predict(steps=len(x_test[0])) # .get_forecast(steps=1, signal_only=False, **kwargs)
forecast = Arima_model.forecast(steps=len(x_test[0]))
print(f'size of test data {len(y_test)} size of forecast data {len(forecast),len(predictions)}')

# Display output graph
plt.plot(predictions,label='Predictions');plt.plot(forecast,label='Forecast')
plt.plot(x_test[0],label='Test data')#Arima_model.plot_predict(dynamic=False)
plt.legend()
plt.show()
'''
# SARIMAX Prediction
Sarimax=SARIMAX(x_train[0],order=(0,1,1)) # With this data the best order seems:(0,1,1)
Sarimax_model=Sarimax.fit()
Sarimax_model.summary()

# SARIMAX Prediction
# Predicting Using Last Window
predictions = Sarimax_model.predict(
                  steps=12,
                  last_window=x[0]
              )#last_window=data.loc[fin_train:]
'''
# Predict: in-data elongation   Forecast: out data prediction