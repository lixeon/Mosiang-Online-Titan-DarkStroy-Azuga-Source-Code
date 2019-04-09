namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class shop : UserControl
    {
        protected Button btnsearch;
        protected DataGrid DataGrid1;
        protected DataList DataList1;
        protected string sqlwhere;
        protected TextBox tbitemname;

        private void btnsearch_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbitemname.Text.ToString());
            base.Response.Redirect("shop.aspx?word=" + str);
        }

        public void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.itemid,a.name,pic,case isbb when 0 then '百寶物品' when 1 then '普通物品' end as isbb, b.name as type,price,gold,comment from web_item a,web_itemtype b where b.used=1 and a.type=b.id" + this.sqlwhere + " order by id desc", "DataGrid1");
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
            if (base.Request.QueryString["type"] != null)
            {
                int num = int.Parse(base.Request.QueryString["type"].ToString());
                this.sqlwhere = " and a.type=" + num;
            }
            if (base.Request.QueryString["word"] != null)
            {
                string str = new system().ChkSql(base.Request.QueryString["word"].ToString().Trim());
                this.sqlwhere = " and a.name like '%" + str + "%'";
            }
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.itemid,a.name,pic,case isbb when 0 then '百寶物品' when 1 then '普通物品' end as isbb, b.name as type,price,gold,comment from web_item a,web_itemtype b where b.used=1 and a.type=b.id" + this.sqlwhere + " order by id desc", "DataGrid1");
                this.DataGrid1.CurrentPageIndex = 0;
                this.DataGrid1.DataBind();
                this.DataList1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype where used=1", "DataGrid1");
                this.DataList1.DataBind();
            }
        }
    }
}
