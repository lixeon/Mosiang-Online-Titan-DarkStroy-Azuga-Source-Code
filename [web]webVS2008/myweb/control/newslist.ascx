<%@ Control Language="c#" AutoEventWireup="false" Codebehind="newslist.ascx.cs" Inherits="web.control.newslist" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<%@ OutputCache Duration="300" VaryByParam="*" %>
<asp:datagrid id="DataGrid1" runat="server" AutoGenerateColumns="False" AllowPaging="True" ShowHeader="False"
	CellPadding="0" GridLines="None" Width="100%" PageSize="30">
	<Columns>
		<asp:TemplateColumn>
			<ItemTemplate>
				<table border="0" cellpadding="2" cellspacing="2" width="100%">
					<tr>
						<td width="5%"  align="left"><IMG src="images/icon_leftmenu_red2.gif"></td>
						<td width="95%" align="left"><a href=viewnews.aspx?id=<%# DataBinder.Eval(Container.DataItem,"id")%> target=_blank>[<%# DataBinder.Eval(Container.DataItem,"name")%>]<%# DataBinder.Eval(Container.DataItem,"title")%>-<%# DataBinder.Eval(Container.DataItem,"author")%>-<%# DataBinder.Eval(Container.DataItem,"adddate")%></a></td>
					</tr>
				</table>
			</ItemTemplate>
		</asp:TemplateColumn>
	</Columns>
	<PagerStyle HorizontalAlign="Right" Mode="NumericPages"></PagerStyle>
</asp:datagrid>
