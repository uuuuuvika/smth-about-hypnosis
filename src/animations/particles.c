#include "../../../main.h"

void particle_animation_init(ParticleAnimation *anim)
{
    anim->time = 0.0;
    anim->num_particles = 900;
}

void particle_animation_draw(ParticleAnimation *anim, MatrixContext *mctx, int x_offset, int width)
{
    const double centerX = x_offset + width / 2.0;
    const double centerY = mctx->height / 2.0;
    const double baseRadius = (mctx->height < width ? mctx->height : width) * 0.25;
    
    const double xAmplitude = 3.4;
    const double yAmplitude = 1.2;
    const double xFreq = 0.015;
    const double yFreq = 0.120;
    const double xSpeed = 0.3;
    const double ySpeed = 0.3;

    for (int f = 0; f < anim->num_particles; f++) {
        double x = centerX - baseRadius * xAmplitude * 
                   sin((anim->time + f * xFreq) * xSpeed);
        double y = centerY - baseRadius * yAmplitude * 
                   cos((anim->time + f * yFreq) * ySpeed);
        
    double fade = (double)f / anim->num_particles;

    uint8_t brightness = 255 * (1.0 - fade);
    uint8_t red = brightness;
    uint8_t green = brightness;
    uint8_t blue = brightness;
    
    if (x >= x_offset && x < x_offset + width && y >= 0 && y < mctx->height) {
        led_canvas_set_pixel(mctx->offscreen_canvas, (int)x, (int)y, red, green, blue);
    }
    }
    
    anim->time += 0.3;
}

//900 - crazy stuff
// void particle_animation_draw(ParticleAnimation *anim, MatrixContext *mctx, int x_offset, int width)
// {
//     const double centerX = x_offset + width / 2.0;
//     const double centerY = mctx->height / 2.0;
//     const double baseRadius = (mctx->height < width ? mctx->height : width) * 0.25;
    
//     const double xAmplitude = 2.4;
//     const double yAmplitude = 1.2;
//     const double xFreq = 0.015;
//     const double yFreq = 0.060;
//     const double xSpeed = 2.2;
//     const double ySpeed = 2.0;

//     for (int f = 0; f < anim->num_particles; f++) {
//         double x = centerX - baseRadius * xAmplitude * 
//                    sin((anim->time + f * xFreq) * xSpeed);
//         double y = centerY - baseRadius * yAmplitude * 
//                    cos((anim->time + f * yFreq) * ySpeed);
        
//     double fade = (double)f / anim->num_particles;

//     uint8_t brightness = 255 * (1.0 - fade);
//     uint8_t red = brightness;
//     uint8_t green = brightness;
//     uint8_t blue = brightness;
    
//     if (x >= x_offset && x < x_offset + width && y >= 0 && y < mctx->height) {
//         led_canvas_set_pixel(mctx->offscreen_canvas, (int)x, (int)y, red, green, blue);
//     }
//     }
    
//     anim->time += 0.002;
// }

// with 800 particles - snake
// void particle_animation_draw(ParticleAnimation *anim, MatrixContext *mctx, int x_offset, int width)
// {
//     const double centerX = x_offset + width / 2.0;
//     const double centerY = mctx->height / 2.0;
//     const double baseRadius = (mctx->height < width ? mctx->height : width) * 0.25;
    
//     const double xAmplitude = 3.0;
//     const double yAmplitude = 1.2;
//     const double xFreq = 0.02;
//     const double yFreq = 0.04;
//     const double xSpeed = 0.2;
//     const double ySpeed = 1.0;

//     for (int f = 0; f < anim->num_particles; f++) {
//         double x = centerX - baseRadius * xAmplitude * 
//                    sin((anim->time + f * xFreq) * xSpeed);
//         double y = centerY - baseRadius * yAmplitude * 
//                    cos((anim->time + f * yFreq) * ySpeed);
        
//        double fade = (double)f / anim->num_particles;

//         uint8_t brightness = 255 * (1.0 - fade);
//         uint8_t red = brightness;
//         uint8_t green = brightness;
//         uint8_t blue = brightness;
    
//         if (x >= x_offset && x < x_offset + width && y >= 0 && y < mctx->height) {
//             led_canvas_set_pixel(mctx->offscreen_canvas, (int)x, (int)y, red, green, blue);
//         }
//     }
//     anim->time += 0.03;
// }

//with 800 particles - circles
// void particle_animation_draw(ParticleAnimation *anim, MatrixContext *mctx, int x_offset, int width)
// {
//     const double centerX = x_offset + width / 2.0;
//     const double centerY = mctx->height / 2.0;
//     const double baseRadius = (mctx->height < width ? mctx->height : width) * 0.25;
//     //const double innerRadius = (mctx->height < width ? mctx->height : width) * 0.15;
    
//     const double xAmplitude = 1.6;
//     const double yAmplitude = 1.2;
//     const double xFreq = 0.015;
//     const double yFreq = 0.03;
//     const double xSpeed = 1.0;
//     const double ySpeed = 0.5;

//     for (int f = 0; f < anim->num_particles; f++) {
//         double x = centerX - baseRadius * xAmplitude * 
//                    sin((anim->time + f * xFreq) * xSpeed);
//         double y = centerY - baseRadius * yAmplitude * 
//                    cos((anim->time + f * yFreq) * ySpeed);

//         // double x_inner = centerX - innerRadius * xAmplitude * 
//         //            sin((anim->time + f * yFreq) * ySpeed);
//         // double y_inner = centerY - innerRadius * yAmplitude * 
//         //            cos((anim->time + f * xFreq) * xSpeed);
        
//         double fade = (double)f / anim->num_particles;

//         uint8_t brightness = 255 * (1.0 - fade);
//         uint8_t red = brightness;
//         uint8_t green = brightness;
//         uint8_t blue = brightness;
    
//         if (x >= x_offset && x < x_offset + width && y >= 0 && y < mctx->height) {
//             led_canvas_set_pixel(mctx->offscreen_canvas, (int)x, (int)y, red, green, blue);
//         }

//         // if (x_inner >= x_offset && x_inner < x_offset + width && y_inner >= 0 && y_inner < mctx->height) {
//         //     led_canvas_set_pixel(mctx->offscreen_canvas, (int)x_inner, (int)y_inner, red, green, blue);
//         // }
//     } 
//     anim->time += 0.03;
// }