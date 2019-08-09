# Font rendering from a texture atlas

Overview:

1. Decide which font to use and find/create a texture atlas that follows the format like here.

2. Format is a box 25X25 in pixels, geometry can be placed anywhere.

3. What makes this so efficient? - minimal amount of code, single gpu submission via SDL_UpdateTexture & SDL_RenderCopy.

4. Every character has its own entity and data associated with it creating unlimited flexibility.

5. 500< LOC simple, no TTF or other BS.

# How It Looks ?

![A Snapshot](snapshot.PNG?raw=true "Snapshot")