#! /usr/bin/env python
import sys,os
import os.path as osp
from os.path import join
from glob import glob
import numpy as np
from time import time
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import EllipseCollection, LineCollection
from time import time

if len(sys.argv)<2:
    print('Please provide the path to the simulation\'s output folder.')
    sys.exit(1)

infolder=sys.argv[1]
outfolder=infolder
picfolder=osp.join(outfolder,'pics')

arrows='nu'
snaps=100		# numbers or set of snapshots to be processed
frame_rate=10

def getParameters(folder):
	name=join(folder,'Parameters_final.dat')
	if osp.exists(name):	column=1
	else:
		name=join(folder,'parameters.par')
		if osp.exists(name):	column=2
		else:	print('Error: Couldn\'t find parameter file.')
	with open(name,'r') as parfile:
	    for line in parfile:
		    s = line.split()
		    if s == []: s = ['']
		    if s[0] == 'np': Np = eval(s[column])		# number of particles
		    elif s[0] == 'v0': v0 = eval(s[column])		# active velocity
	R=np.loadtxt(osp.join(folder,'posinit.dat'),usecols=(3,))
	R_box=abs(np.loadtxt(join(folder,'box.dat'),usecols=(0,))[0])
	return {'Np':Np, 'v0':v0, 'radius':R, 'R_box':R_box}	# Np,v0,R,R_box

if not osp.exists(outfolder):
    os.makedirs(outfolder)
if not osp.exists(picfolder):
    os.makedirs(picfolder)
pardic=getParameters(infolder)
R=pardic['R_box']
L=2.*R
Range=R
radius=pardic['radius']
v0=pardic['v0']
new_snaps=snaps
tNs=len(glob(join(infolder,'PosVel','PosVel*')))
if type(snaps) is int:
	Nmin=max(0,tNs-snaps); new_snaps=range(Nmin,tNs)
Ns=len(new_snaps)
print('found %d snapshots, plotting %d to %d' % (tNs,new_snaps[0],new_snaps[-1]))

t0 = time()
for j in range(Ns):
	print('\r%s\rplotting %d of %d files... (elapsed time: %.3f)' \
			% (' '*20,j,Ns,time()-t0))
	sys.stdout.flush()
	
	i=new_snaps[j]
	posname=osp.join(infolder,'PosVel','PosVel%d.dat'%i)
	data=np.loadtxt(posname, usecols=(0,1,2,3,4))
	pos=data[:,0:2]
	ar=np.zeros((data[:,2:4].shape))
	c='red'
	if arrows=='vel':
		ar=data[:,2:4].T
	if arrows=='nu':
		ar=[np.cos(data[:,2]),np.sin(data[:,2])]
		c='blue'

	fig=plt.figure(figsize=(6,6))
	ax=plt.axes([0,0,1,1],frameon=False)
	ax.set_xticks(())
	ax.set_yticks(())
	cic=EllipseCollection(heights=2*radius,widths=2*radius,angles=np.zeros((pos.shape[0])),offsets=pos, \
			units='x',transOffset=ax.transData,facecolors='none',edgecolors='grey',alpha=1)
	ax.add_collection(cic)
	ax.quiver(pos[:,0],pos[:,1],ar[0],ar[1], \
					color=c,scale=1,units='x')
	ax.text(-0.98*R,-0.98*R,str(j))
	
	plt.axis('scaled')
	plt.axis([-Range,Range,-Range,Range])
	plotname=join(picfolder,'pic_%d.png'%j)
	plt.savefig(plotname,dpi=200)
	plt.close()

os.system(f'ffmpeg -y -r {frame_rate} -i "{picfolder}/pic_%d.png" "{outfolder}/video.mp4"')

