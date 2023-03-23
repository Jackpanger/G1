**1. Explain how a Transform-based steganography technique works and how it differs from substitution-based steganography. (4 points)**

>The transform-based steganography technique hides the information by modifying the transform domain coefficients using mathematical operations like Fourier Transform. 
>
>The substitution-based technique replaces insignificant bits of the original file with message data in a manner that the least amount of distortion is caused. Transform-based steganography is to modify transform domain by mathematical operations but more robust and complex. 

**2. Research and describe 3 common techniques used in steganalysis. (6 points)**

>1. statistical analysis: This is analyzed by comparing a set of unmodified files with the version covered information. 
>2. Machine learning: This is by training a machine learning model to learn the features and then use them to classify media. Algorithms such as SVM or FLD.
>3. Noise floor consistency analysis: advanced technique compared to statistical analysis. This is trying to detect the modification by noise profiles because, in practice, steganography tries to resemble white noise rather than emulate the actual noise.
>
>