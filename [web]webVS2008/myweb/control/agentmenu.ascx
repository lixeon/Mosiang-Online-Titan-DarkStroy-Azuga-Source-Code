<%@ Control Language="c#" AutoEventWireup="false" Codebehind="agentmenu.ascx.cs" Inherits="web.control.agentmenu" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<%@ OutputCache Duration="300" VaryByParam="*" %>
<script>
var is_ie = document.all ? true : false;
var is_ff = window.addEventListener ? true : false;
function refreshmainframe(e) {
	e = e ? e : window.event;
	actualCode = e.keyCode ? e.keyCode : e.charCode;
	if(actualCode == 116 && parent.main) {
		parent.main.location.reload();
		if(is_ie) {
			e.keyCode = 0;
			e.returnValue = false;
		} else {
			e.preventDefault();
		}
	}
}
function get_cookie(name) {
	cookiename = name + '=';
	cookiepos = document.cookie.indexOf(cookiename);
	if(cookiepos != -1) {
		cookiestart =cookiepos+cookiename.length;
		cookieend = document.cookie.indexOf(';', cookiestart);
		if(cookieend == -1) {
			cookieend = document.cookie.length;
		}
		return unescape(document.cookie.substring(cookiestart, cookieend));
	}
	return '';
}
function set_cookie(name, value) {
	expires = new Date();
	expires.setTime(expires.getTime() + 2592000);
	document.cookie = name + "=" + value + "; path=/; expires=" + expires.toGMTString();
}
function $(id) {
	return document.getElementById(id);
}
var collapsed=get_cookie('asiaryl_collapse');
function collapse_change(menucount) {
	if($('menu_' + menucount).style.display == 'none') {
		$('menu_' + menucount).style.display = '';collapsed = collapsed.replace('[' + menucount + ']' , '');
		$('menuimg_' + menucount).src = '../images/admin/menu_reduce.gif';
	} else {
		$('menu_' + menucount).style.display = 'none';collapsed += '[' + menucount + ']';
		$('menuimg_' + menucount).src = '../images/admin/menu_add.gif';
	}
	set_cookie('asiaryl_collapse', collapsed);
}
</script>
<table width="146" border="0" cellspacing="0" align="center" cellpadding="0" class="leftmenulist"
	style="MARGIN-BOTTOM: 5px">
	<tr class="leftmenutext" align="left">
		<td>
			<div align="left">
				<a href="../default.aspx" target="_blank"><b>網站首頁</b></a>&nbsp;&nbsp; <a href="default.aspx">
					<b>後台首頁</b></a>
			</div>
		</td>
	</tr>
</table>
<div id="home"><FONT face="宋体"></FONT>
	<%=strmenu%>
</div>
<FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT>
