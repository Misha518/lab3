import numpy as np
import matplotlib.pyplot as plt
fig2=plt.figure()
ax1=plt.subplot('151')
data = np.loadtxt("Ishod.txt")
data2 = np.loadtxt("Steppolinom.txt")
ax1.plot(data[:,0],data[:,1],'r.')
ax1.plot(data2[:,0],data2[:,1],lw=2)

ax2=plt.subplot('153')
data = np.loadtxt("SecondNewton.txt")
ax2.plot([0,1,2,3,4,5],[11,12.5,10.9,10,8.7,10.7],'ro')
ax2.plot(data[:,0],data[:,1])

ax2=plt.subplot('152')
data = np.loadtxt("FirstNewton.txt")
ax2.plot([0,1,2,3,4,5],[25.4,11.9,10.2,8.2,11.5,10],'ro')
ax2.plot(data[:,0],data[:,1])

ax2=plt.subplot('154')
data = np.loadtxt("Lagranzh.txt")
ax2.plot([0,1,2,3,4,5,6,7,8,9,10,11],[10.7,13.1,11.8,14.1,12.4,10.1,10.9,14.5,14.1,11.5,8.6,11],'ro')
ax2.plot(data[:,0],data[:,1])

ax2=plt.subplot('155')
data = np.loadtxt("Steppolinom.txt")
ax2.plot(data[:,0],data[:,1])
plt.show()
