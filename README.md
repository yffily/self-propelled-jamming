# Self-propelled Jamming

## Description

Brownian dynamics code for self-propelled particles. Developed to study jamming and motility-induced phase separation in self-propelled particles. Used in the following papers:

- _Active jamming: Self-propelled soft particles at high density_, Henkes, Fily & Marchetti, Phys. Rev. E 84, 040301 (2011), [doi:10.1103/PhysRevE.84.040301](doi.org/10.1103/PhysRevE.84.040301).
- _Athermal Phase Separation of Self-Propelled Particles with No Alignment_, Fily & Marchetti, Phys. Rev. Lett. 108, 235702 (2012), [doi:10.1103/PhysRevLett.108.235702](doi.org/10.1103/PhysRevLett.108.235702).
- _Freezing and phase separation of self-propelled disks_, Fily, Henkes & Marchetti, Soft Matter 10, 2132 (2014), [doi:10.1039/C3SM52469H](doi.org/10.1039/C3SM52469H).
- _Minimal model of active colloids highlights the role of mechanical interactions in controlling the emergent behavior of active matter_, Marchetti, Fily, Henkes, Patch & Yllanes, Current Opinion in Colloid & Interface Science 21, 34 (2016), [doi:10.1016/j.cocis.2016.01.003](doi.org/10.1016/j.cocis.2016.01.003).

## Use

To run a Brownian dynamics simulation:

- To compile the c++ code, run `make` in the root directory. This should create an executable called `execute` in the root folder.
- To tweak the simulation settings, edit `parameters.par` in a text editor.
- To run the simulation, execute `./execute parameters.par` in the root folder. This should create a folder called `output` containing the results of the simulation.
- To create a video of the simulation, run `python create_video.py output`. The snapshots are stored in `output/pics`. Creating the snapshots requires matplotlib. Creating the video requires ffmpeg.

## Authors

Yaouen Fily & Silke Henkes.
