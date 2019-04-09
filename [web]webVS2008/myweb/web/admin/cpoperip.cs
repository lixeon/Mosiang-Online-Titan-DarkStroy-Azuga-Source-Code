namespace web.admin
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpoperip : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected DataGrid DataGrid1;
        protected Label lblid;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected TextBox tbip;

        private void btnadd_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbip.Text.ToString());
            new DataProviders().ExecuteSql("insert into TB_OPERIPGAME (ipadress,ipregdate) values ('" + str + "',getdate())");
            base.Response.Redirect("cpoperip.aspx");
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            int num = int.Parse(this.lblid.Text);
            string str = new system().ChkSql(this.tbip.Text.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { "update TB_OPERIPGAME set ipadress='", str, "' where ipidx=", num }));
            this.btnedit.Visible = false;
            this.btnadd.Visible = true;
            base.Response.Redirect("cpoperip.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            new DataProviders().ExecuteSql("delete from TB_OPERIPGAME where ipidx=" + num);
            base.Response.Redirect("cpoperip.aspx");
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from TB_OPERIPGAME where ipidx=" + num);
            if (reader.Read())
            {
                this.lblid.Text = num.ToString();
                this.tbip.Text = reader["ipadress"].ToString();
                this.btnedit.Visible = true;
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
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
            new system().loadConfig(0);
            new WebLogic().isadmin();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from TB_OPERIPGAME", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
