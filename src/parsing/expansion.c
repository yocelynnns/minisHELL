/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:54:52 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/17 23:57:13 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*handle_exit_status(t_process *proc)
{
	char	*env_value;

	env_value = ft_itoa(proc->mini->exit);
	proc->i += 2;
	return (env_value);
}

char	*handle_dollar_sign(t_process *proc)
{
	char	*start;
	char	*env_value;

	proc->i++;
	if (ft_isalnum(proc->str[proc->i]) || proc->str[proc->i] == '_')
	{
		start = proc->str + proc->i;
		while (proc->str[proc->i] && (ft_isalnum(proc->str[proc->i]) \
		|| proc->str[proc->i] == '_'))
			proc->i++;
		start = ft_strndup(start, proc->str + proc->i - start);
		if (!*start)
			return (free(start), ft_strcjoin(proc->result, '$'));
		env_value = get_env_value(start, proc->mini->env);
		free(start);
		return (env_value);
	}
	return (ft_strcjoin(proc->result, '$'));
}

char	*expand_env_variable(t_process *proc)
{
	char	*start;
	char	*env_value;

	start = ++proc->i + proc->str;
	while (proc->str[proc->i] && !ft_strchr("\\\"\'$ ", proc->str[proc->i]))
		proc->i++;
	start = ft_strndup(start, proc->str + proc->i - start);
	if (!*start)
		return (free(start), ft_strcjoin(proc->result, '$'));
	env_value = get_env_value(start, proc->mini->env);
	free(start);
	return (env_value);
}

void	append_env_value(t_process *proc, char *env_value, int free_value)
{
	char	*temp;

	if (!proc->result)
		proc->result = ft_strdup("");
	if (env_value)
	{
		temp = proc->result;
		proc->result = ft_strjoin(proc->result, env_value);
		free(temp);
		if (free_value)
			free(env_value);
	}
	else
		proc->result = ft_strcjoin(proc->result, '\0');
}

char	*expand_variable(t_process *proc)
{
	char	*env_value;
	int		it;

	proc->mini->flag = 1;
	it = 0;
	if (proc->str[proc->i + 1] == '?')
	{
		it = 1;
		env_value = handle_exit_status(proc);
	}
	else if (proc->str[proc->i] == '$')
		env_value = handle_dollar_sign(proc);
	else
		env_value = expand_env_variable(proc);
	append_env_value(proc, env_value, it);
	return (proc->result);
}
