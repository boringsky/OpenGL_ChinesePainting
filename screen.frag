#version 330 core


#define SORT_SIZE  3

in vec2 TexCoords;
out vec4 color;




////
////
//// median
////

float sort[SORT_SIZE];
float medians[SORT_SIZE];

// [0., 1.] -> [0, 255]
//float quant(float x)
//{
//    x = clamp(x, 0., 1.);
//    return floor(x * 255.);
//}

float pack(vec3 c)
{
    float lum = (c.x + c.y + c.z) * (1. / 3.);
    
    return lum;
}

vec3 unpack(float x)
{
    return vec3(x);
}

#define SWAP(a,b) { float t = sort[a]; sort[a] = sort[b]; sort[b] = t; }
void bubble_sort(int num)// 简单的冒泡排序
{
    // 把最小值移到最左边
    for(int j = 0; j < num; ++j)
    {
        for(int i= num-1; i >j; --i)
        {
            if(sort[i] < sort[i-1])
            {
                SWAP(i, i-1);
            }
        }
    }
}

const vec2 iResolution = vec2(800*2, 600*2);

////
////
//// median over
////



////
//// noise diffusion
////
////


const vec2 texSize = vec2(256., 256.);

 vec2 vUV;

vec4 quant(vec4 cl, float n)
{
    cl.x = floor(cl.x * 255./n)*n/255.;
    cl.y = floor(cl.y * 255./n)*n/255.;
    cl.z = floor(cl.z * 255./n)*n/255.;
    
    return cl;
}

////
//// noise diffusion over
////
////



////
//// original
////
////

uniform sampler2D screenTexture;
const float offset = 1.0 / 1000;

uniform sampler2D noiseTexture;

uniform float uQuantLevel;   // 2-6
uniform float uWaterPower;   // 8-64
////
////
////
////
void main()
{
    // the original texture
//    color = texture( screenTexture, TexCoords);
    
    
    //diffusion
    
    vec4 noiseColor = uWaterPower * texture(noiseTexture, vUV);
    vec2 newUV = vec2(vUV.x + noiseColor.x / texSize.x, vUV.y + noiseColor.y / texSize.y);
    vec4 fColor = texture(screenTexture, TexCoords + newUV/600);
    
//    color = fColor;
    color = quant(fColor, 255./pow(2., uQuantLevel));
    
//    vec4 color = vec4(1., 1., .5, 1.);
//    gl_FragColor = color;
    
    
    //blur
    //                vec4 sample0,sample1,sample2,sample3,sample4,sample5,sample6,sample7;
    //
    //                //the trick is fstep
    //                //set proper value could lead to good effection
    //                float fstep= 0.002;
    //                sample0=texture(screenTexture,TexCoords+ vec2(-fstep,-fstep));
    //                sample1=texture(screenTexture,TexCoords+ vec2(+fstep,-fstep));
    //                sample2=texture(screenTexture,TexCoords+ vec2(+fstep,+fstep));
    //                sample3=texture(screenTexture,TexCoords+ vec2(-fstep,+fstep));
    //                sample4=texture(screenTexture,TexCoords+ vec2(-fstep,0));
    //                sample5=texture(screenTexture,TexCoords+ vec2(+fstep,0));
    //                sample6=texture(screenTexture,TexCoords+ vec2(0,-fstep));
    //                sample7=texture(screenTexture,TexCoords+ vec2(0,+fstep));
    //
    //                vec4 fcolor=(sample0+sample1+sample2+sample3+sample4+sample5+sample6+sample7) / 8.0;
    //                color =  fcolor;
    
    
    
    // the kernel texture
    //        vec2 offsets[9] = vec2[](
    //                                 vec2(-offset, offset),  // top-left
    //                                 vec2(0.0f,    offset),  // top-center
    //                                 vec2(offset,  offset),  // top-right
    //                                 vec2(-offset, 0.0f),    // center-left
    //                                 vec2(0.0f,    0.0f),    // center-center
    //                                 vec2(offset,  0.0f),    // center-right
    //                                 vec2(-offset, -offset), // bottom-left
    //                                 vec2(0.0f,    -offset), // bottom-center
    //                                 vec2(offset,  -offset)  // bottom-right
    //                                 );
    //
    //        float kernel[9] = float[](
    //                                  1, 1, 1,
    //                                  1, 8, 1,
    //                                  1, 1, 1
    //                                  );
    //
    //        vec4 sampleTex[9];
    //        for(int i = 0; i < 9; i++)
    //        {
    //            sampleTex[i] = texture(screenTexture, TexCoords.st + offsets[i]);
    //        }
    //
    //        vec3 col = vec3(0.0);
    //        for(int i = 0; i < 9; i++)
    //            col += vec3(sampleTex[i]) * kernel[i];
    //
    //        col = col/12;
    //        color = vec4(col, 1.0);
    
    //    color = (vec4(col, 1.0) + fcolor) / 2;
    
    
    
    
    //median filter
    //    vec2 ooRes = vec2(1.) / iResolution.xy;
    //    //SORT_SIZE个列
    //        for (int j=0; j<SORT_SIZE; j++)
    //        {
    //            //SORT_SIZE个行
    //            for (int i=0; i<SORT_SIZE; i++)
    //            {
    //                vec2 uv = (gl_FragCoord.xy + vec2(i,j)-vec2(SORT_SIZE/2)) * ooRes;
    //                float c = pack( texture(screenTexture,uv).rgb );
    //
    //                sort[i] = c;
    //            }
    //            // 针对某列进行纵向排序
    //            bubble_sort( SORT_SIZE);
    //
    //            //保存该列的中值
    //            float m = sort[(SORT_SIZE/2)];
    //
    //            medians[j] = m;
    //        }
    //
    //        for (int i=0; i<SORT_SIZE; i++)
    //        {
    //            sort[i] = medians[i];
    //        }
    //        //对上一步 SORT_SIZE个列中值 进行横向排序
    //        bubble_sort( SORT_SIZE);
    //        // 提取中值
    //        color = vec4(unpack(sort[SORT_SIZE/2]),1.0);
    
    
    
    
    
    //    color = (vec4(col, 1.0) + vec4(unpack(sort[SORT_SIZE/2]),1.0))/2;
    
    
    
    
    
    
    
    
    //            float blurSizeH = 1.0 / 2000.0;
    //            float blurSizeV = 1.0 / 2000.0;
    //            vec4 sum = vec4(0.0);
    //            for (int x = -4; x <= 4; x++)
    //                for (int y = -4; y <= 4; y++)
    //                    sum += texture(
    //                                   screenTexture,
    //                                   vec2(TexCoords.x + x * blurSizeH, TexCoords.y + y * blurSizeV)
    //                                   ) / 81.0;
    //            color = sum;
    
    
    
    
    
}