"""Growth Rate over pair differential technique"""

# refresh data loader
mm20=list(df.iloc[1,2:-1])
mm21=list(df.iloc[2,2:-1])
mm22=list(df.iloc[3,2:-1])
mm23=list(df.iloc[4,2:-1])
mm24=list(df.iloc[5,2:-1])
mm25=list(df.iloc[6,2:-1])
#print(mm20)
l=len(mm20)#13
size_list=[mm20,mm21,mm22,mm23,mm24,mm25]
size_list_1=[];size_list_2=[]
hh=interval=6
print(f'\n\n *** General growth rate considering 1 hour pair data*** ')
# Making a list of growth for every size available
diff_list=[];GR_list=[]
for i in range(len(size_list)):
  for a in range (l-1):
    diff.append(round(size_list[i][a+1]-size_list[i][a],2))
    #print(diff)
  diff_list.append(diff)
  GR_list.append(round(statistics.mean(diff),2))
  diff=[]

hh=12
print(f'\n---Growth rate/ growth per hours over {hh} hours for each sizes from 20 to 25mm(in list)--- 7 am to 7pm')
print(GR_list);#print(diff_list)
# Growth Rate curve
size_id=['mm20','mm21','mm22','mm23','mm24','mm25']
plt.figure(figsize =(25,5))
for i in range (len(diff_list)):

  plt.subplot(1,len(diff_list),i+1)
  plt.xlabel("Hours")
  plt.ylabel("Diameter(mm)")
  plt.plot(diff_list[i])
  plt.title(f"Growth rate@{size_id[i]}")

#To get generalaised growth rate for PD technique ###
import statistics
print(diff_list)
#plt.plot(PDT_GR,'x')
from sklearn.cluster import KMeans
def k_centroid(PDT_GR):
  PDT_GR=np.array(PDT_GR)
  kmeans=KMeans(n_clusters=2,n_init='auto',max_iter=300).fit(PDT_GR.reshape(-1,1))
  cent=kmeans.cluster_centers_
  return min(cent)
#print('Growth rate for each sizes using Mean',GR_list);#print(diff_list)
cent_list=[]
for i in diff_list:
  cent=k_centroid(i)
  cent=round(cent[0],2)
  cent_list.append(cent)
print('Growth rate for each sizes using K mean',cent_list)