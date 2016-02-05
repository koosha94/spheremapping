#version 400

    in vec3 texcoords;
    uniform samplerCube cube_texture;
    out vec4 frag_colour;
    
    vec4 cubeToLatLon(samplerCube cubemap, vec3 inUV) {
        vec3 cubmapTexCoords;
        const float PI = 3.141592653589793238462643383;
        if(inUV.z == -1 || inUV.z == 1  )
        {
            cubmapTexCoords.x = inUV.x*sqrt(1 - ( (inUV.y * inUV.y)/2 ) - ( (inUV.z * inUV.z)/2 ) + ( ( (inUV.y * inUV.y) * (inUV.z * inUV.z))/3));
            cubmapTexCoords.y= inUV.y*sqrt(1 - ( (inUV.z * inUV.z)/2 ) - ( (inUV.x * inUV.x)/2 ) + ( ( (inUV.z * inUV.z) * (inUV.x * inUV.x))/3));
            cubmapTexCoords.z = inUV.z; 

            return texture(cubemap, cubmapTexCoords);
        }
        if(inUV.y == -1 || inUV.y == 1  )
        {
            cubmapTexCoords.x = inUV.x*sqrt(1 - ( (inUV.y * inUV.y)/2 ) - ( (inUV.z * inUV.z)/2 ) + ( ( (inUV.y * inUV.y) * (inUV.z * inUV.z))/3));
            //cubmapTexCoords.x = inUV.x;
            //cubmapTexCoords.y= inUV.y*sqrt(1 - ( (inUV.z * inUV.z)/2 ) - ( (inUV.x * inUV.x)/2 ) + ( ( (inUV.z * inUV.z) * (inUV.x * inUV.x))/3));
            cubmapTexCoords.y = inUV.y;
            cubmapTexCoords.z = inUV.z*sqrt(1 - ( (inUV.x * inUV.x)/2 ) - ( (inUV.y * inUV.y)/2 ) + ( ( (inUV.x * inUV.x) * (inUV.y * inUV.y))/3));
            //cubmapTexCoords.z = inUV.z;
            return texture(cubemap, cubmapTexCoords);
        }
        if(inUV.x == -1 || inUV.x == 1  )
        {
            //cubmapTexCoords.x = inUV.x*sqrt(1 - ( (inUV.y * inUV.y)/2 ) - ( (inUV.z * inUV.z)/2 ) + ( ( (inUV.y * inUV.y) * (inUV.z * inUV.z))/3));
            cubmapTexCoords.x = inUV.x;
            cubmapTexCoords.y= inUV.y*sqrt(1 - ( (inUV.z * inUV.z)/2 ) - ( (inUV.x * inUV.x)/2 ) + ( ( (inUV.z * inUV.z) * (inUV.x * inUV.x))/3));
            //cubmapTexCoords.y = inUV.y;
            cubmapTexCoords.z = inUV.z*sqrt(1 - ( (inUV.x * inUV.x)/2 ) - ( (inUV.y * inUV.y)/2 ) + ( ( (inUV.x * inUV.x) * (inUV.y * inUV.y))/3));
            //cubmapTexCoords.z = inUV.z;
            return texture(cubemap, cubmapTexCoords);
        }
    }

    void main () {
      //frag_colour = texture (cube_texture, texcoords);
      frag_colour = cubeToLatLon (cube_texture, texcoords);
    }