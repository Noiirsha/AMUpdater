#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <string>

#define screen_x 1366
#define screen_y 768

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Updater", screen_x, screen_y, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	// AMUpdater BG
	SDL_Surface* s_bg = IMG_Load("./Graphics/amupdater_bg.tga");
	SDL_Texture* t_bg = SDL_CreateTextureFromSurface(renderer, s_bg);
	SDL_RenderTexture(renderer, t_bg, nullptr, nullptr);

	// AMUpdater Window
	float w_window, h_window;
	SDL_Texture* t_window = IMG_LoadTexture(renderer, "./Graphics/amupdater_window.tga");
	SDL_GetTextureSize(t_window, &w_window, &h_window);
	SDL_FRect r_window = {
		(screen_x - w_window) / 2.0f,
		(screen_y - h_window) / 2.0f,
		w_window,
		h_window
	};
	SDL_RenderTexture(renderer, t_window, nullptr, &r_window);

	// AMUpdater Logo
	float w_logo, h_logo;
	SDL_Texture* t_logo = IMG_LoadTexture(renderer, "./Graphics/amupdater_banner.tga");
	SDL_GetTextureSize(t_logo, &w_logo, &h_logo);
	SDL_FRect r_logo = {
		506,
		166,
		w_logo,
		h_logo
	};
	SDL_RenderTexture(renderer, t_logo, nullptr, &r_logo);

	// AMUpdater LLoading
	float w_l_loading, h_l_loading;
	SDL_Texture* t_l_loading = IMG_LoadTexture(renderer, "./Graphics/amupdater_loading_l.tga");
	SDL_GetTextureSize(t_l_loading, &w_l_loading, &h_l_loading);
	SDL_FRect r_l_loading = {
	375,
	410,
	w_l_loading,
	h_l_loading
	};
	SDL_RenderTexture(renderer, t_l_loading, nullptr, &r_l_loading);

	// AMUpdater RLoading
	float w_r_loading, h_r_loading;
	SDL_Texture* t_r_loading = IMG_LoadTexture(renderer, "./Graphics/amupdater_loading_r.tga");
	SDL_GetTextureSize(t_r_loading, &w_r_loading, &h_r_loading);
	SDL_FRect r_r_loading = {
	930,
	410,
	w_r_loading,
	h_r_loading
	};
	SDL_RenderTexture(renderer, t_r_loading, nullptr, &r_r_loading);

	// AMUpdater LBar
	float w_lbar, h_lbar;
	SDL_Texture* t_lbar = IMG_LoadTexture(renderer, "./Graphics/amupdater_pbar_s.tga");
	SDL_GetTextureSize(t_lbar, &w_lbar, &h_lbar);
	SDL_FRect r_lbar = {
	433,
	432,
	w_lbar,
	h_lbar
	};
	SDL_RenderTexture(renderer, t_lbar, nullptr, &r_lbar);

	// CBar
	float w_cbar, h_cbar;
	SDL_Texture* t_cbar = IMG_LoadTexture(renderer, "./Graphics/amupdater_pbar_c.tga");
	SDL_GetTextureSize(t_cbar, &w_cbar, &h_cbar);
	SDL_FRect r_cbar = {
	438,
	432,
	w_cbar*61.5,
	h_cbar
	};
	SDL_RenderTexture(renderer, t_cbar, nullptr, &r_cbar);

	// RBar
	float w_rbar, h_rbar;
	SDL_Texture* t_rbar = IMG_LoadTexture(renderer, "./Graphics/amupdater_pbar_e.tga");
	SDL_GetTextureSize(t_rbar, &w_rbar, &h_rbar);
	SDL_FRect r_rbar = {
	928,
	432,
	w_rbar,
	h_rbar
	};
	SDL_RenderTexture(renderer, t_rbar, nullptr, &r_rbar);



	SDL_RenderPresent(renderer);

	SDL_Delay(10000);

	// Destroyer
	SDL_DestroyTexture(t_bg);
	SDL_DestroySurface(s_bg);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
	return 0;
}
