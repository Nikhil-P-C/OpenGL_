#shader vertex
    #version 330 core   
    layout (location =0) in vec3 aPos;  
    layout (location =1) in vec2 aTexCoord;
    out vec2 v_texCoord;
    uniform mat4 u_PVM;
    void main (){   
        v_texCoord = vec2(aTexCoord);
        gl_Position =u_PVM* vec4(aPos.x,aPos.y,aPos.z, 1.0);   
    }

#shader fragment
    #version 330 core   
    out vec4 fcolor;
    in vec2 v_texCoord;

    uniform float u_color;
    uniform sampler2D u_texture;

    void main (){   
        fcolor =texture(u_texture,v_texCoord);   
    }