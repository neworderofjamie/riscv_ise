import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt

data = np.fromfile("courtyard_walk_stereo.calib/courtyard.bin", dtype=np.uint32)

# Identify timesteps
timesteps = np.where(data & (1<<31))[0]

# Split data by timestep
data_timesteps = np.split(data, timesteps)[1:]

last_timestep = data_timesteps[-1][0] & ((1 << 31) - 1)

data_frames = np.zeros((last_timestep + 1, 320, 320), dtype=np.int8)

for d in data_timesteps:
    t = d[0] & ((1 << 31) - 1)
    x = (d[1:] >> 9) & 0x1FF
    y = d[1:] & 0x1FF
    p = (d[1:] >> 17).astype(int) - 1
    data_frames[t,y,x] = p
    
    #plt.imshow(data_frames[i])
    #plt.show()
  
fig, axis = plt.subplots()


current_data_frame = data_frames[0].astype(float)
spike_image = axis.imshow(current_data_frame, interpolation="nearest", cmap="jet", vmin=-1, vmax=1.0)

def updatefig(frame):
    global spike_image, current_data_frame, data_frames

    # Decay image data
    current_data_frame *= 0.7

    # Loop through all timesteps that occur within frame
    current_data_frame += data_frames[frame]

    # Set image data
    spike_image.set_array(current_data_frame)

    
    # Return list of artists which we have updated
    return [spike_image]

# Play animation
ani = animation.FuncAnimation(fig, updatefig, range(data_frames.shape[0]), interval=33, blit=True, repeat=True)
plt.show()