#neural network processing
I used the SFML library on a PC in visual studio with these files, and a few other things, like folders, fonts and images, which can be found in the files themselves.

To train data, load and normalize your images via fiterator and normalize, then create a network and run feedforward and backprop with your training data. Then call saveweights. The neural network is adapted from this: https://vimeo.com/19569529

The next step would be to rewrite some of this code for opencv, so it can run on the pi, after transferring weights.txt and running loadweights on the pi.
