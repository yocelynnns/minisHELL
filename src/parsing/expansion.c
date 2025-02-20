/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:54:52 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/20 19:41:58 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_first(t_process *proc, char **start, char **env_value);
char	*ft_second(t_process *proc, char **start, char **env_value);
char	*ft_third(t_process *proc, char **env_value, int *it);

char	*expand_variable(t_process *proc, t_minishell *mini)
{
	int		it;
	char	*start;
	char	*env_value;

	proc->mini->flag = 1;
	it = 0;
	env_value = NULL;
	if (proc->str[proc->i + 1] == '?')
	{
		it = 1;
		env_value = ft_itoa(mini->exit);
		proc->i += 2;
	}
	else if (proc->str[proc->i] == '$')
	{
		proc->i++;
		proc->result = ft_first(proc, &start, &env_value);
	}
	else
		proc->result = ft_second(proc, &start, &env_value);
	if (!proc->result)
		proc->result = ft_strdup("");
	proc->result = ft_third(proc, &env_value, &it);
	return (proc->result);
}

char	*ft_third(t_process *proc, char **env_value, int *it)
{
	char	*temp;

	temp = NULL;
	if (*env_value)
	{
		temp = proc->result;
		proc->result = ft_strjoin(proc->result, *env_value);
		free(temp);
		if (*it)
		{
			free(*env_value);
			*it = 0;
		}
	}
	else
		proc->result = ft_strcjoin(proc->result, '\0');
	return (proc->result);
}

char	*ft_second(t_process *proc, char **start, char **env_value)
{
	*start = ++proc->i + proc->str;
	while (proc->str[proc->i] && !ft_strchr("\\\"\'$ ",
			proc->str[proc->i]))
		proc->i++;
	*start = ft_strndup(*start, proc->str + proc->i - *start);
	if (!*start)
	{
		proc->result = ft_strcjoin(proc->result, '$');
		free(*start);
	}
	*env_value = get_env_value(*start, proc->mini->env);
	free (start);
	return (proc->result);
}

char	*ft_first(t_process *proc, char **start, char **env_value)
{
	if (ft_isalnum(proc->str[proc->i]) || proc->str[proc->i] == '_')
	{
		*start = proc->str + proc->i;
		while (proc->str[proc->i] && \
			(ft_isalnum(proc->str[proc->i]) || proc->str[proc->i] == '_'))
			proc->i++;
		*start = ft_strndup(*start, proc->str + proc->i - *start);
		if (!*start)
		{
			proc->result = ft_strcjoin(proc->result, '$');
			free(*start);
		}
		*env_value = get_env_value(*start, proc->mini->env);
		free(*start);
	}
	else
		proc->result = ft_strcjoin(proc->result, '$');
	return (proc->result);
}
