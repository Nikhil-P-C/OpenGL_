#vertex shader
    
    #version 330 core   
    layout (location =0) in vec2 aPos;   
    void main (){   
        gl_Position = vec4(aPos.x,aPos.y, 0.0, 1.0);   
    }

#fragment shader
    
    #version 330 core   
    out vec4 fcolor;   
    void main (){   
        fcolor = vec4(1.0,0.5, 0.0, 1.0);   
    }