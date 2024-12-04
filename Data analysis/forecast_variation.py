import numpy as np
import matplotlib.pyplot as plt


sun_acc=[3705,3519,3000,1821,203.6,188.8,612,875];sun_forecast=[3400,3400,3000,2400,200,200,600,800]
mon_acc=[5248,4468,515,1210,1475,324,2522];mon_forecast=[4600,4000,500,1200,1400,200,2400]
tue_acc=[1212,3035,753,2059.2,2569.8,1058,4286];tue_forecast=[1200,3000,800,2800,3200,1000,4600]
wed_acc=[427,2846.4,4310.4,2465,3518,270.4,383.4];wed_forecast=[600,2200,4200,2600,2800,200,200]
thu_acc=[550.2,2951.6,4501.7,1109.8,1561.8,87,138];thu_forecast=[500,3000,4400,1000,1200,100,200]
fri_acc=[3813,2950,3637.4,593,831,167];fri_forecast=[3200,2800,3400,600,800,200]
sat_acc=[4147,3982,1375,1767,1188,298,148,881,58];sat_forecast=[4200,4000,1600,1800,1200,200,100,1000,100]


week_acc=[sun_acc,mon_acc,tue_acc,wed_acc,thu_acc,fri_acc,sat_acc] ;week_forecast= [sun_forecast,mon_forecast,tue_forecast,wed_forecast,thu_forecast,fri_forecast,sat_forecast];
print(week_acc)

week=['sun','mon','tue','wed','thu','fri','sat']
for i in range(len(week)):
	plt.plot(week_forecast[i],  label='forecast',marker='o');plt.plot(week_acc[i],label='actual',marker='o')
	plt.legend()
	plt.title(f'Forecast vs Actual weight @ {week[i]}')
	plt.xlabel('Tunnels');plt.ylabel('Weight(KGs)')
	plt.show()

# Subplot
# Read excel and fetch data