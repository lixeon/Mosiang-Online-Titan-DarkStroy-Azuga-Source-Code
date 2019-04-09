namespace web.admin
{
    using System;
    using System.Data;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpguild : Page
    {
        protected Button btnsearch;
        protected DataGrid DataGrid1;
        private static DataSet ds;
        protected TextBox tbguildname;
        protected TextBox tbmastername;

        private void btnsearch_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = system.ChkSql(this.tbguildname.Text.ToString());
            string str2 = system.ChkSql(this.tbmastername.Text.ToString());
            string str3 = "";
            if (str != "")
            {
                str3 = " where guildname like '%" + str + "%'";
            }
            else if (str2 != "")
            {
                str3 = " where mastername like '%" + str2 + "%'";
            }
            else if ((str == "") & (str2 == ""))
            {
                return;
            }
            string mySql = "select * from mhgame..tb_guild" + str3;
            ds = new DataProviders().ExecuteSqlDs(mySql, "DataGrid1");
            for (int i = 0; i < ds.Tables[0].Rows.Count; i++)
            {
                ds.Tables[0].Rows[i]["mastername"] = system.ConvertToBig5(ds.Tables[0].Rows[i]["mastername"].ToString(), 950);
                ds.Tables[0].Rows[i]["guildname"] = system.ConvertToBig5(ds.Tables[0].Rows[i]["guildname"].ToString(), 950);
            }
            this.DataGrid1.DataSource = ds;
            this.DataGrid1.CurrentPageIndex = 0;
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = ds;
            this.DataGrid1.DataBind();
        }

        private void InitializeComponent()
        {
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
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
        }
    }
}
