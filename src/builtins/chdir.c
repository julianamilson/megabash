#include "minishell.h"

static void	new_pwd_env(void)
{
	char	*directory;
	t_env	*env_node;
	t_env	*temp;

	temp = g_megabash.env;
	directory = getcwd(NULL, 0);
	env_node = get_env_node(temp, "PWD");
	if (env_node == NULL)
		env_addback(&g_megabash.env, env_lst_new("PWD", directory));
	else
	{
		free(env_node->content);
		env_node->content = ft_strdup(directory);
	}
	free(directory);
}

static void	new_oldpwd_env(void)
{
	char	*directory;
	t_env	*env_node;
	t_env	*temp;

	temp = g_megabash.env;
	directory = getcwd(NULL, 0);
	env_node = get_env_node(temp, "OLDPWD");
	if (env_node == NULL)
		env_addback(&g_megabash.env, env_lst_new("OLDPWD", directory));
	else
	{
		free(env_node->content);
		env_node->content = ft_strdup(directory);
	}
	free(directory);
}

static void	home_execute(void)
{
	t_env	*env_node;
	t_env	*temp;

	temp = g_megabash.env;
	env_node = get_env_node(temp, "HOME");
	if (env_node == NULL)
	{
		error_message("megabash: cd: HOME not set", 1);
	}
	else
	{
		new_oldpwd_env();
		chdir(env_node->content);
		new_pwd_env();
	}
}

static void	dash_execute(void)
{
	char	*directory;
	char	*new_dir;
	t_env	*env_node;
	t_env	*temp;

	temp = g_megabash.env;
	env_node = get_env_node(temp, "OLDPWD");
	if (env_node == NULL)
	{
		error_message("megabash: cd: OLDPWD not set", 1);
	}
	else
	{
		directory = ft_strdup(env_node->content);
		new_oldpwd_env();
		chdir(directory);
		new_dir = getcwd(NULL, 0);
		new_pwd_env();
		if(env_node->content)
			ft_putendl_fd(new_dir, 0);
		free(directory);
		free(new_dir);
	}
}

static void    expand_and_redirect(char *dir)
{
    char    *directory;
    t_env    *env_node;
    t_env    *temp;
    char    *sub_dir;

    sub_dir = ft_substr(dir, 2, ft_strlen(dir));
    temp = g_megabash.env;
    env_node = get_env_node(temp, "HOME");
    if (env_node == NULL)
        error_message("megabash: cd: HOME not set", 1);
    else
    {
    	directory = ft_strdup(env_node->content);
        new_oldpwd_env();
        chdir(directory);
        new_pwd_env();
    	free(directory);
    }
    if    (chdir(sub_dir))
    {
        ft_putstr_fd("megabash: cd:", 2);
        ft_putstr_fd(sub_dir, 2);
        ft_putendl_fd(": No such file or directory", 2);
    }
    free(sub_dir);
}

void	cd(char **matrix)
{
	if (matrix_size(matrix) > 1)
		ft_putendl_fd("megabash: cd: too many arguments", 2);
	if (matrix_size(matrix) == 0
		|| !ft_strncmp(g_megabash.cmd_list->content[1], "~", 2))
		home_execute();
	else if (!ft_strncmp(g_megabash.cmd_list->content[1], "-", 2))
		dash_execute();
	 else
    {
        new_oldpwd_env();
        if (chdir(matrix[1]) && matrix[1][0] != '~')
        {
            ft_putstr_fd("megabash: cd:", 2);
            ft_putstr_fd(matrix[1], 2);
            ft_putendl_fd(": No such file or directory", 2);
        }
        else if (matrix[1][0] == '~')
        {
            expand_and_redirect(matrix[1]);
        }
        new_pwd_env();
    }
}
