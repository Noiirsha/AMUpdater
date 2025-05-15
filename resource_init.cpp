#include "resource.h"

void initResource() {
    // Resource Implementation
    s_bg = IMG_Load("./Graphics/amupdater_bg.tga");
    t_bg = SDL_CreateTextureFromSurface(renderer, s_bg);

    t_window = IMG_LoadTexture(renderer, "./Graphics/amupdater_window.tga");
    SDL_GetTextureSize(t_window, &w_window, &h_window);
    r_window = {
        (screen_x - w_window) / 2.0f,
        (screen_y - h_window) / 2.0f,
        w_window,
        h_window
    };

    t_logo = IMG_LoadTexture(renderer, "./Graphics/amupdater_banner.tga");
    SDL_GetTextureSize(t_logo, &w_logo, &h_logo);
    r_logo = { 506, 166, w_logo, h_logo };

    t_l_loading = IMG_LoadTexture(renderer, "./Graphics/amupdater_loading_l.tga");
    SDL_GetTextureSize(t_l_loading, &w_l_loading, &h_l_loading);
    r_l_loading = { 375, 410, w_l_loading, h_l_loading };

    t_r_loading = IMG_LoadTexture(renderer, "./Graphics/amupdater_loading_r.tga");
    SDL_GetTextureSize(t_r_loading, &w_r_loading, &h_r_loading);
    r_r_loading = { 930, 410, w_r_loading, h_r_loading };

    t_lbar = IMG_LoadTexture(renderer, "./Graphics/amupdater_pbar_s.tga");
    SDL_GetTextureSize(t_lbar, &w_lbar, &h_lbar);
    r_lbar = { 433, 432, w_lbar, h_lbar };

    t_cbar = IMG_LoadTexture(renderer, "./Graphics/amupdater_pbar_c.tga");
    SDL_GetTextureSize(t_cbar, &w_cbar, &h_cbar);

    t_rbar = IMG_LoadTexture(renderer, "./Graphics/amupdater_pbar_e.tga");
    SDL_GetTextureSize(t_rbar, &w_rbar, &h_rbar);
}