#include	"mlx.h"
#include	"stdio.h"
#include	"stdlib.h"

typedef struct	s_vars
{
	void	*mlx;
	void	*win;
}				t_vars;

typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		x_len;
	int		y_len;
	int		endian;
}				t_data;

typedef struct	s_anim
{
	t_data	*sheet;
	int		anim_time;
	int		timer;
	int		size;
	int		n_spr;
}				t_anim;

typedef struct	s_handler
{
	t_vars	mlx;
	t_data	sheet;
	t_data	sprite;
	t_anim	anim_run;
}				t_handler;

t_data	ft_crop(void *mlx, t_data srcs, int x_off, int y_off)
{
	t_data	sprite;
	int		x;
	int		y;
	unsigned int	*srcaddr;

	sprite.img = mlx_new_image(mlx, 170, 170);
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.x_len, &sprite.endian);
	srcaddr = (unsigned int*)srcs.addr + y_off * srcs.x_len + x_off;
	y = -1;
	x = -1;
	printf("%u", *srcaddr);
	while (++y < 170)
	{
		while (++x < 170)
			*(((unsigned int*)sprite.addr) + x + y * 170) = *(srcaddr + x + y * (srcs.x_len / 4));
		x = -1;
	}
	return (sprite);
}

void	ft_animate(t_handler *h)
{
	static int x;
	if (h->anim_run.timer++ >= h->anim_run.anim_time)
	{
		h->anim_run.n_spr++;
		if (h->anim_run.n_spr == h->anim_run.size)
			h->anim_run.n_spr = 0;
		h->anim_run.timer = 0;
	}

	mlx_put_image_to_window(h->mlx.mlx, h->mlx.win, h->anim_run.sheet[h->anim_run.n_spr].img,++x * 5, 0);

}

int	ft_init_anim(t_anim *anim)
{
	anim->sheet = (t_data *)malloc(sizeof(t_data) * (11));
	anim->size = 10;
	anim->anim_time = 3;
	anim->timer = 0;
	anim->n_spr = 0;
	return (0);
}

int	main(void)
{
	t_handler	h;
	int			i;

	ft_init_anim(&h.anim_run);
	h.mlx.mlx = mlx_init();
	h.mlx.win = mlx_new_window(h.mlx.mlx, 1000, 1000, "Our sheet test");
	h.sheet.img = mlx_xpm_file_to_image(h.mlx.mlx, "sheetmegaman.xpm", &h.sheet.x_len, &h.sheet.y_len);
	h.sheet.addr = mlx_get_data_addr(h.sheet.img, &h.sheet.bpp, &h.sheet.x_len, &h.sheet.endian);
	i = -1;
	while (++i < 10)
		h.anim_run.sheet[i] = ft_crop(h.mlx.mlx, h.sheet, 170 * i, 0);
	// h.sprite = ft_crop(h.mlx.mlx, h.sheet, 0, 0);
	i = -1;
	// while (++i < 5)
	// 	mlx_put_image_to_window(h.mlx.mlx, h.mlx.win, h.anim_run.sheet[i].img, i * 175, 0);
	// mlx_put_image_to_window(h.mlx.mlx, h.mlx.win, h.sheet.img, 0, 0);
	mlx_loop_hook(h.mlx.mlx, ft_animate, &h);
	mlx_loop(h.mlx.mlx);
	return (0);
}