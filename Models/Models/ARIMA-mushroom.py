import numpy as np
import pandas as pd 
import matplotlib.pyplot as plt 

from sklearn.model_selection import train_test_split
from sklearn.gaussian_process import GaussianProcessRegressor
from sklearn.gaussian_process.kernels import RBF, ConstantKernel,ExpSineSquared,RationalQuadratic, WhiteKernel,  DotProduct

# libraries for ADF test to get best nodel order for ARIMA
from statsmodels.tsa.stattools import adfuller
from pmdarima import auto_arima
# libraries ARIMA model
from statsmodels.tsa.arima.model import ARIMA # ARIMA with no seasonal variation
from statsmodels.tsa.statespace.sarimax import SARIMAX # SARIMAX with seasonal change/factors

#import data
file_path="C:/Users/Staff/Documents/Agaricus/Mushroom Forecast/Models/Model data visualisation/Fake data/Fake_data.xlsx"
df = pd.read_excel(file_path)
#print(df[0:5])  # print first 5 rows of the dataframe

mm20=list(df.iloc[1,2:-1])
mm21=list(df.iloc[2,2:-1])
mm22=list(df.iloc[3,2:-1])
mm23=list(df.iloc[4,2:-1])
mm24=list(df.iloc[5,2:-1])
mm25=list(df.iloc[6,2:-1])

size_list=[mm20,mm21,mm22,mm23,mm24,mm25]

# Structuring data for feeding
dataset=np.array(size_list)
# Look at the output entity structure, visualise each batches with input size and out put
#x=np.reshape(dataset,(13,6))

print(f'Data under consideration {dataset} with size {dataset.shape}')
x=dataset[0]
print(f'Input data (20mm) {x} {x.shape}')

#print(' Output shape ',y.shape)

#Splitting the data into train and test set
split=10 # % is the  test set
x_train,x_test= x[0:10],x[ 10:]
print(f'x train {x_train} || x test {x_test}\n')

# Augmented Dickey-Fuller test # it helps see p-value for stationarity <0.05
# tsa:time series analysis

diagnosis= adfuller(x_train)
print(f'\nAugmented Dickey-Fuller test...\nADF STAT: {diagnosis[0]}  || P-value {diagnosis[1]}')
print('Critical Values:')
for key, value in diagnosis[4].items():
  print('\t%s: %.3f' % (key, value))

# Fetching p,d,q value : best model order 
stepwise_fit = auto_arima(x_train, trace=True,suppress_warnings=True) #not subscriptable # tries find out order with minimum AIC score
print(f'Stepwise fit(order){stepwise_fit}\n') # # Best model:  ARIMA(2,0,0)(0,0,0)[0] intercept
#stepwise_fit = auto_arima(y_train, trace=True,suppress_warnings=True)

# Forecast outbound data using the model 
Arima=ARIMA(x_train,order=(2,0,0)) # ARIMA(x_train,order) <<< X
Arima_model=Arima.fit()
Arima_model.summary()

# ARIMA Predictions and Forecast
predictions = Arima_model.predict(steps=len(x_test)) # .get_forecast(steps=1, signal_only=False, **kwargs)
forecast = Arima_model.forecast(steps=len(x_test))
print(f'\nsize of test data {len(x_test)} size of forecast data {len(forecast),len(predictions)}')

# Display output graph
plt.plot(predictions,label='Predictions')
plt.plot(forecast,label='Forecast');plt.plot(x_test,label='Test data')
#Arima_model.plot_predict(dynamic=False)
plt.legend()
plt.show()
'''
# SARIMAX Prediction
Sarimax=SARIMAX(x_train,order=(2,0,0)) # With this data the best order seems:(0,1,1)
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