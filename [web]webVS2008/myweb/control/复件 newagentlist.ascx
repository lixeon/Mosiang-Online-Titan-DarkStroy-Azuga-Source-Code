<%@ OutputCache Duration="300" VaryByParam="*" %>
<%@ Control Language="c#" AutoEventWireup="false" Codebehind="newagentlist.ascx.cs" Inherits="web.control.newagentlist" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table width="278" border="0" cellpadding="0" cellspacing="0"  valign=top>
  <tr>
    <td background="images/gold_top_blue.jpg" height="38"></td>
  </tr>
  <tr>
    <td background="images/gold_top2_blue.jpg" height="20"></td>
  </tr>
  <tr>
    <td background="images/gold_center_blue.jpg" height="163" valign=top><table width="277" border="0">
      <tr >
        <td width="10"></td>
        <td width="241" align=center><b>今日單人累計贊助金幣最高玩家的帳號</b><font color="blue"><b><%=gold%></b></font></td>
        <td width="12"></td>
      </tr> 
      <br><br>
		<%=str%>
	</table>
	</td>
  </tr>
  <tr>
    <td background="images/gold_bottom_blue.jpg" height="27">&nbsp;</td>
  </tr>
</table>