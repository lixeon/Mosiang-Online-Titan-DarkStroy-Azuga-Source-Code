<%@ OutputCache Duration="300" VaryByParam="*" %>
<%@ Control Language="c#" AutoEventWireup="false" Codebehind="agentlist.ascx.cs" Inherits="web.control.agentlist" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<asp:DataGrid id="DataGrid1" runat="server" AutoGenerateColumns="False" BorderColor="#999999"
	BorderWidth="1px" BackColor="White" CellPadding="3" Width="100%" BorderStyle="Solid" ForeColor="Black" GridLines="Vertical">
<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#000099">
</SelectedItemStyle>

<AlternatingItemStyle BackColor="#CCCCCC">
</AlternatingItemStyle>

<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="Black">
</HeaderStyle>

<FooterStyle BackColor="#CCCCCC">
</FooterStyle>

<Columns>
<asp:HyperLinkColumn DataNavigateUrlField="id" DataNavigateUrlFormatString="../viewagent.aspx?id={0}" DataTextField="name" HeaderText="代理姓名">
<ItemStyle Font-Bold="True" HorizontalAlign="Left">
</ItemStyle>
</asp:HyperLinkColumn>
<asp:BoundColumn DataField="nation" HeaderText="地區">
<ItemStyle HorizontalAlign="Left">
</ItemStyle>
</asp:BoundColumn>
<asp:BoundColumn DataField="gold" HeaderText="剩余金幣">
<ItemStyle HorizontalAlign="Left">
</ItemStyle>
</asp:BoundColumn>
</Columns>

<PagerStyle HorizontalAlign="Center" ForeColor="Black" BackColor="#999999">
</PagerStyle>
</asp:DataGrid>
