#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#define screen_x 1366
#define screen_y 768

// Resources
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* s_bg;
SDL_Texture* t_bg;

// AMUpdater Window
SDL_Texture* t_window;
float w_window, h_window;
SDL_FRect r_window;

// AMUpdater Banner
SDL_Texture* t_logo;
float w_logo, h_logo;
SDL_FRect r_logo;

// AMUpdater Loading
SDL_Texture* t_l_loading;
float w_l_loading, h_l_loading;
SDL_FRect r_l_loading;

SDL_Texture* t_r_loading;
float w_r_loading, h_r_loading;
SDL_FRect r_r_loading;

// AMUpdater ProgressBar
SDL_Texture* t_lbar;
float w_lbar, h_lbar;
SDL_FRect r_lbar;

SDL_Texture* t_cbar;
float w_cbar, h_cbar;
SDL_FRect r_cbar;

SDL_Texture* t_rbar;
float w_rbar, h_rbar;
SDL_FRect r_rbar;

// Rotation
SDL_Thread* loading_thread = nullptr;
bool loading_active = true;
float rotation_angle = 0.0f;
float rotation_speed = 1.5f; // 稍微加快旋转速度
SDL_Mutex* rotation_mutex = nullptr;

// TTF
TTF_Font* font_title;
TTF_Font* font_content;
SDL_Color font_color = { 0xff,0xff,0xff,0xff };
std::string title_string = "WANGAN MIDNIGHT MAXIMUM TUNE 5DX Plus";
std::string revision_string = "REV 3.09.01";
std::string content_string = "Strating Initialization ......";

// Func
void clearScreenAndRepaint();
static int loadingThreadFunction(void* data);
void renderLoading();
void renderText();

void initResource() {

    // Rotation Thread
    rotation_mutex = SDL_CreateMutex();
    loading_thread = SDL_CreateThread(loadingThreadFunction, "LoadingThread", NULL);

    // Font
    font_title = TTF_OpenFont("C:/Windows/Fonts/tahoma.ttf", 19);
    font_content = TTF_OpenFont("C:/Windows/Fonts/tahoma.ttf", 11);
    TTF_SetFontStyle(font_title, TTF_STYLE_BOLD);
    TTF_SetFontStyle(font_content, TTF_STYLE_BOLD);

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

    clearScreenAndRepaint();
}

void clearScreenAndRepaint() {
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, t_bg, nullptr, nullptr);
    SDL_RenderTexture(renderer, t_window, nullptr, &r_window);
    SDL_RenderTexture(renderer, t_logo, nullptr, &r_logo);
    SDL_RenderTexture(renderer, t_l_loading, nullptr, &r_l_loading);
    SDL_RenderTexture(renderer, t_r_loading, nullptr, &r_r_loading);

    renderLoading();
    renderText();

}

void renderText() {
    SDL_Surface* s_titleText = TTF_RenderText_Blended(font_title, title_string.c_str(), title_string.length(), font_color);
    SDL_Surface* s_revisionText = TTF_RenderText_Blended(font_title, revision_string.c_str(), revision_string.length(), font_color);
    SDL_Surface* s_contentText = TTF_RenderText_Blended(font_content, content_string.c_str(), content_string.length(), font_color);
    SDL_Texture* t_titleText = SDL_CreateTextureFromSurface(renderer, s_titleText);
    SDL_Texture* t_revisionText = SDL_CreateTextureFromSurface(renderer, s_revisionText);
    SDL_Texture* t_contentText = SDL_CreateTextureFromSurface(renderer, s_contentText);

    float w_tt, h_tt, w_rt, h_rt, w_ct, h_ct;

    SDL_GetTextureSize(t_titleText, &w_tt, &h_tt);
    SDL_FRect r_tt = { (screen_x - w_tt)/2.0f ,282,w_tt + 5,h_tt };
    SDL_RenderTexture(renderer, t_titleText, nullptr, &r_tt);

    SDL_GetTextureSize(t_revisionText, &w_rt, &h_rt);
    SDL_FRect r_rt = { (screen_x - w_rt) / 2.0f ,303,w_rt + 5,h_rt };
    SDL_RenderTexture(renderer, t_revisionText, nullptr, &r_rt);

    SDL_GetTextureSize(t_contentText, &w_ct, &h_ct);
    SDL_FRect r_ct = { 402 , 469 ,w_ct + 3,h_ct };
    SDL_RenderTexture(renderer, t_contentText, nullptr, &r_ct);
}

static int loadingThreadFunction(void* data) {
    while (loading_active) {
        SDL_LockMutex(rotation_mutex);
        rotation_angle += rotation_speed;
        if (rotation_angle >= 360.0f) {
            rotation_angle -= 360.0f;
        }
        SDL_UnlockMutex(rotation_mutex);

        SDL_Delay(16); // 约60FPS
    }
    return 0;
}

void renderLoading() {
    SDL_LockMutex(rotation_mutex);
    float current_angle = rotation_angle;
    SDL_UnlockMutex(rotation_mutex);
    SDL_RenderTextureRotated(renderer, t_l_loading, nullptr, &r_l_loading, current_angle, nullptr, SDL_FLIP_NONE);
    SDL_RenderTextureRotated(renderer, t_r_loading, nullptr, &r_r_loading, current_angle, nullptr, SDL_FLIP_NONE);
}

void renderProgressBar(int precentage) {

    // 渲染进度条中间部分
    if (precentage > 0) {
        SDL_RenderTexture(renderer, t_lbar, nullptr, &r_lbar);

        float progress_width = w_cbar * (precentage / 100.0f) * 61.5f; // 使用浮点数除法
        SDL_FRect r_cbar = {
            438,
            432,
            progress_width,
            h_cbar
        };
        SDL_RenderTexture(renderer, t_cbar, nullptr, &r_cbar);

        // MAX:928
        SDL_FRect r_rbar = {
            (438 + progress_width < 928) ? 438 + progress_width : 928,
            432,
            w_rbar,
            h_rbar
        };
        SDL_RenderTexture(renderer, t_rbar, nullptr, &r_rbar);
    }

}

void destroyResource() {

    loading_active = false;
    if (loading_thread) {
        SDL_WaitThread(loading_thread, NULL);
    }
    if (rotation_mutex) {
        SDL_DestroyMutex(rotation_mutex);
    }

    TTF_CloseFont(font_title);
    TTF_CloseFont(font_content);

    SDL_DestroyTexture(t_bg);
    SDL_DestroySurface(s_bg);
    SDL_DestroyTexture(t_window);
    SDL_DestroyTexture(t_logo);
    SDL_DestroyTexture(t_l_loading);
    SDL_DestroyTexture(t_r_loading);
    SDL_DestroyTexture(t_lbar);
    SDL_DestroyTexture(t_cbar);
    SDL_DestroyTexture(t_rbar);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}