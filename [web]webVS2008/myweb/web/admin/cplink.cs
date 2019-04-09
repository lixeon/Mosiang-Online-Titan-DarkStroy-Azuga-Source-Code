namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cplink : Page
    {
        protected Button btnadd;
        protected DataGrid DataGrid1;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator3;
        protected RequiredFieldValidator RequiredFieldValidator4;
        protected TextBox tbalt;
        protected TextBox tblink;
        protected TextBox tbname;

        private void btnadd_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbname.Text.ToString());
            string str2 = new system().ChkSql(this.tblink.Text.ToString());
            string str3 = new system().ChkSql(this.tbalt.Text.ToString());
            new DataProviders().ExecuteSql("insert into web_link (name,link,alt) values ('" + str + "','" + str2 + "','" + str3 + "')");
            base.Response.Redirect("cplink.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            new DataProviders().ExecuteSql("delete from web_link where id=" + num);
            base.Response.Redirect("cplink.aspx");
        }

        private void InitializeComponent()
        {
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.btnadd.Click += new EventHandler(this.btnadd_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_link", "DataGrid1");
            this.DataGrid1.DataBind();
        }
    }
}
